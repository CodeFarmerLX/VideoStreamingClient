#include "MyRoom.h"
#include "ui_MyRoom.h"
#include "package.h"
#include <QMessageBox>
#include <QHostAddress>
#include <QCameraInfo>
#include "mybarrage.h"

MyRoom::MyRoom(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyRoom)
{
    ui->setupUi(this);

    QCameraInfo info = QCameraInfo::defaultCamera();
    camera = new QCamera(info, this);

    writeSocket = new QUdpSocket(this);

    vs = new VideoSuface(this);
    camera->setViewfinder(vs);

    connect(vs, SIGNAL(frameChanged(QVideoFrame)),
            this, SLOT(frameChangedSlot(QVideoFrame)));

    QAudioFormat format;
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);
    //自动匹配最合适的输入设备
    QAudioDeviceInfo info2 = QAudioDeviceInfo::defaultInputDevice();
    if(!info2.isFormatSupported(format))//实测不会进这里
    {
        format = info2.nearestFormat(format);
    }
    this->input = new QAudioInput(format);
    this->voiceSocket = new QUdpSocket(this);

}

MyRoom::~MyRoom()
{
    delete ui;
}

void MyRoom::on_btnBack_clicked()
{
    ui->labelCameraView->clear();
    camera->stop();
    ui->btnCloseCamera->hide();
    ui->btnOpenCamera->show();

    this->input->stop();

    pack_t packCloseRoom;
    memset(&packCloseRoom, 0, sizeof(packCloseRoom));

    packCloseRoom.type = TYPE_CLOSE_ROOM;
    packCloseRoom.roomId = ui->labelId->text().toInt();

    this->clientSocket->write((const char *)&packCloseRoom, sizeof(packCloseRoom));
}

void MyRoom::showMyRoomView(pack_t &pack,  QTcpSocket *socket)
{   
    this->clientSocket = socket;
    ui->labelId->setText(QString::number(pack.id));
    ui->labelTMoney->setText(QString::number(pack.tmoney));
    ui->labelPoint->setText(QString::number(pack.point));

    QString sname = QString::fromLocal8Bit(pack.name);
    QString sroomname = QString::fromLocal8Bit(pack.roomName);

    ui->labelRoomName->setText(sroomname);
    ui->labelName->setText(sname);

    ui->listWidget->clear();
    ui->textEdit->clear();

    ui->btnCloseCamera->hide();
    ui->btnVoiceClose->hide();

    QWidget * p = this->parentWidget();

    this->show();
    p->hide();
}

void MyRoom::receiveFromLobbyView(int state, pack_t &pack)
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
        QString sname = QString::fromLocal8Bit(pack.name);
        qDebug() << "房主内收到加入名单" << pack.name;
        ui->listWidget->addItem(sname);
        break;
    }
    case 3:
        ui->listWidget->clear();
        break;
    case 4:{
        QMessageBox::information(this, "关闭房间提示", "关闭成功");
        QWidget * p = this->parentWidget();
        p->show();

        this->close();
        break;
    }
    case 5:
        ui->labelPoint->setText(QString::number(pack.point));
        break;
    default:
            break;
    }
}


void MyRoom::clearUsrList()
{
    ui->textEdit->clear();
}

void MyRoom::clearMessageList()
{
    ui->listWidget->clear();
}

void MyRoom::frameChangedSlot(QVideoFrame frame)
{
    //QVideoFrame---->QImage
    frame.map(QAbstractVideoBuffer::ReadOnly);
#if 0

        QImage(uchar *data,         //图片字节数组的首地址
           int width,           //图片的宽度
           int height,          //图片高度
           int bytesPerLine,    //图片每行的字节数
           Format format,       //图片处理格式
           );

#endif
    QImage img(
        frame.bits(),
        frame.width(),
        frame.height(),
        frame.bytesPerLine(),
        QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat())
    );

    //旋转图片180
    QMatrix matrix;
    matrix.rotate(180);
    img = img.transformed(matrix);

    //QImage--->QPixmap
    QPixmap pixMap = QPixmap::fromImage(img);
    //让图片适应label的大小
    pixMap = pixMap.scaled(ui->labelCameraView->size());
    //将图片显示到label上
    ui->labelCameraView->setPixmap(pixMap);

    qDebug() << img.size();
    //将图片信息以广播组的形式发送出去
    QByteArray byte;
    QBuffer buffer(&byte);           //缓存
    img.save(&buffer, "JPEG");

    qDebug() << byte.size();
    //将图片信息写入到制定端口

    QHostAddress hip = this->clientSocket->peerAddress();

    unsigned short port = getPort();

    writeSocket->writeDatagram(byte,
          QHostAddress("224.0.0.223"), port);
    qDebug() << "write....";

}

void MyRoom::on_btnOpenCamera_clicked()
{
    camera->start();

    ui->btnOpenCamera->hide();
    ui->btnCloseCamera->show();
}

void MyRoom::on_btnCloseCamera_clicked()
{
    ui->labelCameraView->clear();
    camera->stop();
    ui->btnCloseCamera->hide();
    ui->btnOpenCamera->show();
}

void MyRoom::gift(const QString & text)
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

void MyRoom::on_btnVoiceOpen_clicked()
{
    audioInputIODevice = this->input->start();
    connect(audioInputIODevice,SIGNAL(readyRead()),
            this,SLOT(voiceReadSlot()));

    ui->btnVoiceOpen->hide();
    ui->btnVoiceClose->show();
}

void MyRoom::voiceReadSlot()
{
    qDebug() << "声音输入设备收到声音";
    voice pack;
    memset(&pack,0,sizeof(pack));
    pack.lens = audioInputIODevice->read(pack.data,1024);
    voiceSocket->writeDatagram((const char *)&pack, sizeof(pack),QHostAddress("224.0.0.224"), this->port);
}

void MyRoom::on_btnVoiceClose_clicked()
{
    this->input->stop();
    ui->btnVoiceClose->hide();
    ui->btnVoiceOpen->show();
}
