#include "UsrRoom.h"
#include "ui_UsrRoom.h"
#include <QMessageBox>
#include <QDebug>
#include <QBuffer>
#include <QImageReader>
#include "usrsleep.h"
#include <QPoint>
#include "mybarrage.h"

UsrRoom::UsrRoom(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UsrRoom)
{
    ui->setupUi(this);

    //配置QAudioInput播放参数QAudioFormat
    QAudioFormat format;
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);
    //判断当前的音频输入设备是否支持QAudioFormat配置，如果不支持，获取支持的最接近的配置信息
    QAudioDeviceInfo info2 = QAudioDeviceInfo::defaultInputDevice();
    if(!info2.isFormatSupported(format))//实测不会进这里
    {
        format = info2.nearestFormat(format);
    }

    this->output = new QAudioOutput(format);
    audioOutputIODevice = output->start();//只打开一次
}

UsrRoom::~UsrRoom()
{
    delete ui;
}

void UsrRoom::showUsrRoom(pack_t &pack,  QTcpSocket *socket)
{
    readSocket = new QUdpSocket(this);

    connect(this->readSocket, SIGNAL(readyRead()),
            this, SLOT(readyreadSlot()));

    voiceSocket = new QUdpSocket(this);
    connect(this->voiceSocket, SIGNAL(readyRead()),
            this,SLOT(voiceReadSlot()));

    this->setRoomAddress(QString::fromLocal8Bit(pack.roomIpAddress));
    this->setRoomPort(pack.roomPort);
    signed short port = pack.roomPort;
    this->readSocket->bind(QHostAddress::AnyIPv4, port,
                                        QUdpSocket::ShareAddress
                                        | QUdpSocket::ReuseAddressHint);
    QString ipAddress = QString::fromLocal8Bit(pack.roomIpAddress);
    this->readSocket->joinMulticastGroup(QHostAddress("224.0.0.223"));

    qDebug() << "ip" << ipAddress << "   port" << port;

    voiceSocket->bind(QHostAddress::AnyIPv4,port,
                     QUdpSocket::ShareAddress
                     | QUdpSocket::ReuseAddressHint);
    voiceSocket->joinMulticastGroup(QHostAddress("224.0.0.224"));


    this->clientSocket = socket;
    ui->labelId->setText(QString::number(pack.id));
    ui->labelTMoney->setText(QString::number(pack.tmoney));
    ui->labelPoint->setText(QString::number(pack.point));
    ui->labelRoomId->setText(QString::number(pack.roomId));

    QString sname = QString::fromLocal8Bit(pack.name);
    QString sroomname = QString::fromLocal8Bit(pack.roomName);

    ui->labelRoomName->setText(sroomname);
    ui->labelRoomName->setFont(QFont("SimHei",20,100));
    QPalette pa;
    pa.setColor(QPalette::Foreground,QColor(255,0,0));
    ui->labelRoomName->setPalette(pa);

    ui->labelName->setText(sname);

    ui->textEdit->clear();
    ui->listWidget->clear();

    QWidget * p = this->parentWidget();

    this->show();
    p->hide();
}

void UsrRoom::readyreadSlot()
{
    qDebug() << "............";
    quint64 size = readSocket->pendingDatagramSize();
    //直接读取异步监听时接收到的数据报
    QByteArray buff;
    // 字节矩阵，大小用qint64描述，适合操作1大量数据2字符串数组3二进制01数据，节省内存，更加安全
    //一般用const char*初始化
    // 默认采用深拷贝，可指定为浅拷贝
    buff.resize(size);
    //为缓冲区规定一个maxsize（没必要用那么大，刚好装下图像就够了）
    readSocket->readDatagram(buff.data(),buff.size());
    // 数据报只支持一维操作
    QBuffer buffer(&buff);

    //提供用Qbytearray读写内存中IO缓存区的接口，若无传参内部默认自动创建一个Qbytearray；

    //对IO缓存区读写操作等效于像IO设备读写数据

    //操作QBuffer像在操作文件（其实原理都差不多，各自都是内存中一块特殊区域嘛）
    QImageReader reader(&buffer,"JPEG");
    //可读入磁盘文件、设备文件中的图像、以及其他图像数据如pixmap和image，相比较更加专业。
    //buffer属于设备文件一类，
    //专业地读取设备文件的图像数据。
    QImage image = reader.read();
    //read()方法用来读取设备图像，也可读取视频，读取成功返回QImage*，否则返回NULL
    //格式转换
    QPixmap pix = QPixmap::fromImage(image);
    ui->labelCamera->setPixmap(pix.scaled(ui->labelCamera->size()));
}

//发送消息
void UsrRoom::on_btnSentText_clicked()
{    
    if(ui->lineEdit->text().isEmpty())
    {
        QMessageBox::critical(this, "发送消息", "发送内容不能为空");
        return;
    }

    QString smessage = ui->lineEdit->text();
    ui->lineEdit->clear();

    int id = ui->labelId->text().toInt();
    QString sname = ui->labelName->text();
    int roomid = ui->labelRoomId->text().toInt();

    pack_t packSentM;
    memset(&packSentM, 0, sizeof(packSentM));

    packSentM.type = TYPE_SENT_TEXT_IN_ROOM;
    packSentM.id = id;
    packSentM.roomId = roomid;

    QByteArray bname = sname.toLocal8Bit();
    const char * cname = bname.data();
    qstrcpy(packSentM.name, cname);

    QByteArray bmessage = smessage.toLocal8Bit();
    const char * cmessage = bmessage.data();
    qstrcpy(packSentM.message, cmessage);

    this->clientSocket->write((const char *)&packSentM, sizeof(packSentM));
}

void UsrRoom::receiveFromLobbyView(int state, pack_t &pack)
{
    switch(state){

    case 1:{//收到群聊消息
        QString smessage = QString::fromLocal8Bit(pack.message);
        QString sname = QString::fromLocal8Bit(pack.name);
        QString info = sname + " : " + smessage;
        ui->textEdit->append(info);

        gift(smessage);

        Mybarrage *lb = new Mybarrage(smessage,ui->frame);
        lb->show();
        lb->myMove();

        break;
    }
    case 2:{
        delete readSocket;
        delete voiceSocket;

        QMessageBox::information(this, "退出房间", "退出成功");
        QWidget * p = this->parentWidget();
        p->show();

        this->close();
        break;
    }
    case 3:{
        QMessageBox::information(this, "退出房间", "退出失败");
        break;
    }
    case 4:{//更新房间内用户列表
        QString sname = QString::fromLocal8Bit(pack.name);
        qDebug() << "用户房间内增加用户" << pack.name;
        ui->listWidget->addItem(sname);
        break;
    }
    case 5://清空用户列表
        ui->listWidget->clear();
        break;
    case 6:{
        QMessageBox::information(this, "退出房间", "房主已下线");
        QWidget * p = this->parentWidget();
        p->show();
        this->close();
        break;
        }
    case 7:
        ui->labelPoint->setText(QString::number(pack.point));
        ui->labelTMoney->setText(QString::number(pack.tmoney));
        break;
    default:
            break;
    }
}

void UsrRoom::clearUsrList()
{
    ui->textEdit->clear();
}

void UsrRoom::clearMessageList()
{
    ui->listWidget->clear();
}

void UsrRoom::on_btnBack_clicked()
{
    ui->labelCamera->clear();
    this->readSocket->close();

    int id = ui->labelId->text().toInt();
    int roomid = ui->labelRoomId->text().toInt();

    pack_t packLeaveRoom;
    memset(&packLeaveRoom, 0, sizeof(packLeaveRoom));

    packLeaveRoom.type = TYPE_EXIT_ROOM;
    packLeaveRoom.id =id;
    packLeaveRoom.roomId = roomid;

    this->clientSocket->write((const char *)& packLeaveRoom, sizeof(packLeaveRoom));
}

//人民币飞机
void UsrRoom::on_pushButton_3_clicked()
{
    if(ui->labelTMoney->text().toInt() < 1)
    {
        QMessageBox::critical(this, "送礼提示", "您的余额不足1\n请到用户界面充值");
        return;
    }

    pack_t packGift;

    memset(&packGift, 0, sizeof(packGift));

    QString smessage = "送给主播一个飞机";

    int id = ui->labelId->text().toInt();
    QString sname = ui->labelName->text();
    int roomid = ui->labelRoomId->text().toInt();

    pack_t packSentM;
    memset(&packSentM, 0, sizeof(packSentM));

    packSentM.type = TYPE_SENT_TEXT_IN_ROOM;
    packSentM.id = id;
    packSentM.roomId = roomid;

    QByteArray bname = sname.toLocal8Bit();
    const char * cname = bname.data();
    qstrcpy(packSentM.name, cname);

    QByteArray bmessage = smessage.toLocal8Bit();
    const char * cmessage = bmessage.data();
    qstrcpy(packSentM.message, cmessage);

    this->clientSocket->write((const char *)&packSentM, sizeof(packSentM));

    usrSleep(100);

    packGift.type = TYPE_SENT_FLOWER;

    packGift.id = id;
    packGift.roomId = roomid;

    packGift.money = 1;
    packGift.point = 0;

    this->clientSocket->write((const char *)&packGift, sizeof(packGift));
}

//人民币火箭
void UsrRoom::on_pushButton_4_clicked()
{
    if(ui->labelTMoney->text().toInt() < 100)
    {
        QMessageBox::critical(this, "送礼提示", "您的余额不足100\n请到用户界面充值");
        return;
    }

    pack_t packGift;

    memset(&packGift, 0, sizeof(packGift));

    QString smessage = "送给主播一个火箭";

    int id = ui->labelId->text().toInt();
    QString sname = ui->labelName->text();
    int roomid = ui->labelRoomId->text().toInt();

    pack_t packSentM;
    memset(&packSentM, 0, sizeof(packSentM));

    packSentM.type = TYPE_SENT_TEXT_IN_ROOM;
    packSentM.id = id;
    packSentM.roomId = roomid;

    QByteArray bname = sname.toLocal8Bit();
    const char * cname = bname.data();
    qstrcpy(packSentM.name, cname);

    QByteArray bmessage = smessage.toLocal8Bit();
    const char * cmessage = bmessage.data();
    qstrcpy(packSentM.message, cmessage);

    this->clientSocket->write((const char *)&packSentM, sizeof(packSentM));

    usrSleep(100);

    packGift.type = TYPE_SENT_FLOWER;

    packGift.id = id;
    packGift.roomId = roomid;

    packGift.money = 100;
    packGift.point = 0;

    this->clientSocket->write((const char *)&packGift, sizeof(packGift));
}

//积分鲜花
void UsrRoom::on_pushButton_clicked()
{
    if(ui->labelPoint->text().toInt() < 100)
    {
        QMessageBox::critical(this, "送礼提示", "您的积分不足100");
        return;
    }

    pack_t packGift;

    memset(&packGift, 0, sizeof(packGift));

    QString smessage = "送给主播一朵鲜花";

    int id = ui->labelId->text().toInt();
    QString sname = ui->labelName->text();
    int roomid = ui->labelRoomId->text().toInt();

    pack_t packSentM;
    memset(&packSentM, 0, sizeof(packSentM));

    packSentM.type = TYPE_SENT_TEXT_IN_ROOM;
    packSentM.id = id;
    packSentM.roomId = roomid;

    QByteArray bname = sname.toLocal8Bit();
    const char * cname = bname.data();
    qstrcpy(packSentM.name, cname);

    QByteArray bmessage = smessage.toLocal8Bit();
    const char * cmessage = bmessage.data();
    qstrcpy(packSentM.message, cmessage);

    this->clientSocket->write((const char *)&packSentM, sizeof(packSentM));

    usrSleep(100);

    packGift.type = TYPE_SENT_FLOWER;

    packGift.id = id;
    packGift.roomId = roomid;

    packGift.money = 0;
    packGift.point = 100;

    this->clientSocket->write((const char *)&packGift, sizeof(packGift));
}

//积分布偶
void UsrRoom::on_pushButton_2_clicked()
{
    if(ui->labelPoint->text().toInt() < 2000)
    {
        QMessageBox::critical(this, "送礼提示", "您的积分不足2000");
        return;
    }

    pack_t packGift;

    memset(&packGift, 0, sizeof(packGift));

    QString smessage = "送给主播一个布偶";

    int id = ui->labelId->text().toInt();
    QString sname = ui->labelName->text();
    int roomid = ui->labelRoomId->text().toInt();

    pack_t packSentM;
    memset(&packSentM, 0, sizeof(packSentM));

    packSentM.type = TYPE_SENT_TEXT_IN_ROOM;
    packSentM.id = id;
    packSentM.roomId = roomid;

    QByteArray bname = sname.toLocal8Bit();
    const char * cname = bname.data();
    qstrcpy(packSentM.name, cname);

    QByteArray bmessage = smessage.toLocal8Bit();
    const char * cmessage = bmessage.data();
    qstrcpy(packSentM.message, cmessage);

    this->clientSocket->write((const char *)&packSentM, sizeof(packSentM));

    usrSleep(100);

    packGift.type = TYPE_SENT_FLOWER;

    packGift.id = id;
    packGift.roomId = roomid;

    packGift.money = 0;
    packGift.point = 2000;

    this->clientSocket->write((const char *)&packGift, sizeof(packGift));
}

//礼物动画
void UsrRoom::gift(const QString & text)
{
    if(text == "送给主播一个飞机")
    {
        QPixmap img = QPixmap(":/new/prefix1/feiji.jpg");
        Mybarrage *lb = new Mybarrage(img,this);
        lb->show();
        lb->myMove();
    }
    else if(text == "送给主播一个火箭")
    {
        QPixmap img = QPixmap(":/new/prefix1/huojian.png");
        Mybarrage *lb = new Mybarrage(img,this);
        lb->show();
        lb->myMove();
    }
}

void UsrRoom::voiceReadSlot()
{
    qDebug() << "收到主播的声音";
    voice pack;
    memset(&pack,0,sizeof(pack));
    voiceSocket->readDatagram((char *)&pack, sizeof(pack));//没这句，只触发一次信号
    audioOutputIODevice->write(pack.data,pack.lens);
}
