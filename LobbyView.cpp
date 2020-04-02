#include "LobbyView.h"
#include "ui_LobbyView.h"
#include <QMessageBox>
#include <QTimer>
#include <QTime>
#include <QDebug>

#include "mysleep.h"

LobbyView::LobbyView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LobbyView)
{
    ui->setupUi(this);

    this->myRoomView = new MyRoom(this);
    this->usrRoomView = new UsrRoom(this);

    QWidget *p = this->parentWidget();
    this->clientSocket = new QTcpSocket(p);

    this->id = 0;
    this->name = " ";
}

LobbyView::~LobbyView()
{
    delete ui;
    delete this->clientSocket;
}

void LobbyView::on_btnBack_clicked()
{
    QString name = ui->textName->text();
    int id = ui->textId->text().toInt();

    pack_t packLogOut;
    memset(&packLogOut, 0, sizeof(packLogOut));

    packLogOut.type = TYPE_LOGOUT_BACK;
    QByteArray bName = name.toLocal8Bit();
    const char * cName = bName.data();

    packLogOut.id = id;
    qstrcpy(packLogOut.name, cName);

    this->clientSocket->write((const char*)&packLogOut, sizeof(packLogOut));
}

void LobbyView::showLobbyView(pack_t & pack, QTcpSocket *socket)
{
    this->clientSocket = socket;

    int id = pack.id;
    this->setId(id);
    QString sid = QString::number(id);

    QString sname = QString::fromLocal8Bit(pack.name);
    this->setName(sname);

    int tmoney = pack.tmoney;
    QString stmoney = QString::number(tmoney);

    int point = pack.point;
    QString spoint = QString::number(point);

    ui->textId->setText(sid);
    ui->textName->setText(sname);
    ui->textTMoney->setText(stmoney);
    ui->textPoint->setText(spoint);

    ui->listWidgetRoom->clear();

    this->show();
    this->on_btnUpdate_clicked();
}

void LobbyView::on_btnClose_clicked()
{
    QString name = ui->textName->text();
    int id = ui->textId->text().toInt();

    pack_t packLogOut;
    memset(&packLogOut, 0, sizeof(packLogOut));

    packLogOut.type = TYPE_LOGOUT;
    QByteArray bName = name.toLocal8Bit();
    const char * cName = bName.data();

    packLogOut.id = id;
    qstrcpy(packLogOut.name, cName);

    this->clientSocket->write((const char*)&packLogOut, sizeof(packLogOut));
}

void LobbyView::receiveFromStartView(int state, pack_t &pack)
{
    QWidget * p = 0;
    switch(state){
    case 1://下线成功
        QMessageBox::information(this, "下线提示", "下线成功");
        p = this->parentWidget();
        this->hide();
        p->close();
        break;
    case 2://下线失败
        QMessageBox::critical(this, "下线提示", "下线失败");
        break;
    case 3://下线成功返回
        QMessageBox::information(this, "下线提示", "下线成功");
        p = this->parentWidget();
        this->hide();
        p->show();
        break;
    case 4://刷新房间成功-没有房间
        ui->listWidgetRoom->clear();
        ui->listWidgetRoom->addItem("当前没有房间");
        break;
    case 5:{//刷新房间成功-有房间

        QString sid = QString::number(pack.id);
        QString sname = QString::fromLocal8Bit(pack.name);
        QString sroomname = QString::fromLocal8Bit(pack.roomName);

        QString info = sid + "\t\t" + sname + "\t\t" + sroomname;

        ui->listWidgetRoom->addItem(info);
    }
        break;
    case 6://创建房间成功
        this->myRoomView->setPort(pack.roomPort);
        this->myRoomView->showMyRoomView(pack, this->clientSocket);
        break;
    case 7://加入房间成功
        this->usrRoomView->showUsrRoom(pack, this->clientSocket);
        break;
    case 8://接受房间内消息
        this->usrRoomView->receiveFromLobbyView(1, pack);
        this->myRoomView->receiveFromLobbyView(1, pack);
        break;
    case 9://房间列表清空
        ui->listWidgetRoom->clear();
        break;
    case 10://退出成功
        this->usrRoomView->receiveFromLobbyView(2, pack);
        break;
    case 11://退出失败
        this->usrRoomView->receiveFromLobbyView(3, pack);
        break;
    case 12://更新房间内用户
        qDebug() << pack.name;
        this->usrRoomView->receiveFromLobbyView(4, pack);
        this->myRoomView->receiveFromLobbyView(2, pack);
        break;
    case 13://清空房间内用户列表
        this->usrRoomView->receiveFromLobbyView(5, pack);
        this->myRoomView->receiveFromLobbyView(3, pack);
        break;
    case 14://向房主发送房间关闭信息
        this->myRoomView->receiveFromLobbyView(4, pack);
        break;
    case 15://向房间内用户发送房主关闭房间的信息
        this->usrRoomView->receiveFromLobbyView(6, pack);
        break;
    case 16://向房间内用户发送房主关闭房间的信息
        ui->textTMoney->setText(QString::number(pack.tmoney));
        ui->textPoint->setText(QString::number(pack.point));
        break;
    case 17://充值成功
        QMessageBox::information(this, "充值", "充值成功");
        ui->textTMoney->setText(QString::number(pack.tmoney));
        break;
    case 18://充值失败
        QMessageBox::critical(this, "充值", "充值失败");
        break;
    case 19://加入房间失败
        QMessageBox::critical(this, "加入房间", "房间不存在或主播已下线");
        break;
    case 20://房主收礼物
        this->myRoomView->receiveFromLobbyView(5, pack);
        break;
    case 21://游客送礼
        this->usrRoomView->receiveFromLobbyView(7, pack);
        break;
    default:
        break;
    }
}

//刷新房间
void LobbyView::on_btnUpdate_clicked()
{
    ui->listWidgetRoom->clear();

    pack_t packUpdate;
    memset(&packUpdate, 0, sizeof(packUpdate));

    packUpdate.type = TYPE_UPDATE_ROOMS;

    this->clientSocket->write((const char *)&packUpdate, sizeof(packUpdate));
}

//开启新的直播间
void LobbyView::on_btnStartNewRoom_clicked()
{
    this->myRoomView->clearUsrList();
    this->myRoomView->clearMessageList();

    this->usrRoomView->clearUsrList();
    this->usrRoomView->clearMessageList();

    if(ui->lineNewRoomName->text().isEmpty())
    {
        QMessageBox::critical(this, "创建房间", "房间名不能为空");
        return;
    }

    QString newRoomName = ui->lineNewRoomName->text();
    ui->lineNewRoomName->clear();


    pack_t packCreateRoom;
    memset(&packCreateRoom, 0, sizeof(packCreateRoom));

    packCreateRoom.type = TYPE_CREATE_ROOM;

    packCreateRoom.id = ui->textId->text().toInt();

    QString sname = ui->textName->text();
    qDebug() << "开启新直播间按钮的名字测试" << sname;
    QByteArray bName = sname.toLocal8Bit();
    const char * cName = bName.data();
    qstrcpy(packCreateRoom.name, cName);

    QString sroomname = newRoomName;
//    QString sroomname = sname;
    QByteArray bRoomName = sroomname.toLocal8Bit();
    const char * cRoomName = bRoomName.data();
    qstrcpy(packCreateRoom.roomName, cRoomName);

    packCreateRoom.tmoney = ui->textTMoney->text().toInt();
    packCreateRoom.point = ui->textPoint->text().toInt();
//    QString sipAddress = this->clientSocket->peerAddress();

    this->clientSocket->write((const char *)&packCreateRoom, sizeof(packCreateRoom));
}

//双击进入房间
void LobbyView::on_listWidgetRoom_doubleClicked(const QModelIndex &index)
{
    int id = ui->textId->text().toInt();
    QString info = index.data().toString();
    if(info == "当前没有房间")
    {
        QMessageBox::critical(this, "加入房间", "不能加入该房间\n请刷新列表后重试");
        return;
    }

    QString sroomId = info.section("\t",0,0);
    int roomId = sroomId.toInt();

    if(id == roomId)
    {
        QMessageBox::critical(this, "加入房间", "不能加入自己的房间");
        return;
    }

    pack_t packIntoRoom;
    memset(&packIntoRoom, 0, sizeof(packIntoRoom));

    packIntoRoom.type = TYPE_JOIN_ROOM;

    packIntoRoom.id = id;
    packIntoRoom.roomId = roomId;

    this->clientSocket->write((const char *)&packIntoRoom, sizeof(packIntoRoom));
}

//ID进入房间
void LobbyView::on_btnJoinRoom_clicked()
{
    int id = ui->textId->text().toInt();
    int roomid = ui->lineRoomId->text().toInt();

    if(id == roomid)
    {
        QMessageBox::critical(this, "加入房间", "不能加入自己的房间");
        return;
    }

    pack_t packIntoRoom;
    memset(&packIntoRoom, 0, sizeof(packIntoRoom));

    packIntoRoom.type = TYPE_JOIN_ROOM;

    packIntoRoom.id = id;
    packIntoRoom.roomId = roomid;

    this->clientSocket->write((const char *)&packIntoRoom, sizeof(packIntoRoom));
}

//充值
void LobbyView::on_btnReMoney_clicked()
{
    if(ui->lineReMoney->text().isEmpty() || ui->lineReMoney->text().toInt() <= 0)
    {
        QMessageBox::information(this, "充值提示", "充值不能为空或小于0元");
        return;
    }
    QMessageBox::information(this, "充值提示", "正在充值... ...");

    int remoney = ui->lineReMoney->text().toInt();
    int id = ui->textId->text().toInt();

    ui->lineReMoney->clear();

    pack_t packReMoney;
    memset(&packReMoney, 0, sizeof(packReMoney));

    packReMoney.type = TYPE_RECHARGE;
    packReMoney.id = id;
    packReMoney.money = remoney;

    this->clientSocket->write((const char *)&packReMoney, sizeof(packReMoney));

}

//刷新大厅用户信息
void LobbyView::on_btnUpdateUsrInfo_clicked()
{
    int id = ui->textId->text().toInt();
    pack_t packUpUsrInfo;
    memset(&packUpUsrInfo, 0, sizeof(packUpUsrInfo));

    packUpUsrInfo.type = TYPE_UP_USR_INFO;
    packUpUsrInfo.id = id;

    this->clientSocket->write((const char *)&packUpUsrInfo, sizeof(packUpUsrInfo));
}
