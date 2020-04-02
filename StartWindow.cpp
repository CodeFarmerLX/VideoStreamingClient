#include "StartWindow.h"
#include "ui_StartWindow.h"
#include <QHostAddress>
#include <QDebug>
#include <QMessageBox>

StartWindow::StartWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StartWindow)
{
    ui->setupUi(this);
    this->regView = new RegView(this);
    this->lobView = new LobbyView(this);
    this->findPwView = new FindPw(this);

    ui->linePw->setEchoMode(QLineEdit::Password);

    this->clientSocket = new QTcpSocket(this);
    this->clientSocket->connectToHost(QHostAddress::LocalHost,9090);

    connect(this->clientSocket, SIGNAL(readyRead()),
            this, SLOT(readyReadSlot()));
}

StartWindow::~StartWindow()
{
    delete ui;
}

//注册
void StartWindow::on_btnRegist_clicked()
{
    this->regView->showRegView(this->clientSocket);
}

//读取套接字信息槽函数
void StartWindow::readyReadSlot()
{
    qDebug() << "mainwindow readySlot";
    pack_t pack;
    memset(&pack, 0, sizeof(pack));

    while(!this->clientSocket->atEnd()){

    this->clientSocket->read((char*)&pack, sizeof(pack));

    switch(pack.type){
    case TYPE_LOGIN_FAIL_NAME:
        QMessageBox::information(this, "登录", "用户名不存在");
        break;
    case TYPE_LOGIN_FAIL_PW:
        QMessageBox::critical(this, "登录", "密码错误");
        break;
    case TYPE_LOGIN_FAIL:
        QMessageBox::critical(this, "登录", "登录失败");
        break;
    case TYPE_LOGIN_FAIL_ONLINE:
        QMessageBox::critical(this, "登录", "用户已在线");
        break;
    case TYPE_LOGIN_SUCCESS:
        this->hide();
        this->lobView->showLobbyView(pack, this->clientSocket);
        break;
    case TYPE_REGIST_SUCCESS:
        this->regView->receiveFromStartView(1);
        break;
    case TYPE_REGIST_FAIL:
        this->regView->receiveFromStartView(2);
        break;
    case TYPE_LOGOUT_FAIL:
        this->lobView->receiveFromStartView(2, pack);
        break;
    case TYPE_LOGOUT_SUCCESS:
        this->lobView->receiveFromStartView(1, pack);
        break;
    case TYPE_LOGOUT_SUCCESS_BACK:
        this->lobView->receiveFromStartView(3, pack);
        break;
    case TYPE_UPDATE_ROOMS_SUCCESS_NO_ROOM:
        this->lobView->receiveFromStartView(4, pack);
        break;
    case TYPE_UPDATE_ROOMS_SUCCESS:
        this->lobView->receiveFromStartView(5, pack);
        break;
    case TYPE_CREATE_ROOM_SUCCESS:
        this->lobView->receiveFromStartView(6, pack);
        break;
    case TYPE_JOIN_ROOM_SUCCESS:
        this->lobView->receiveFromStartView(7, pack);
        break;
    case TYPE_GET_TEXT_IN_ROOM:
        this->lobView->receiveFromStartView(8, pack);
        break;
    case TYPE_UPDATE_ROOMS_CLEAR_TEXT:
        this->lobView->receiveFromStartView(9, pack);
        break;
    case TYPE_EXIT_ROOM_SUCCESS:
        this->lobView->receiveFromStartView(10, pack);
        break;
    case TYPE_EXIT_ROOM_FAIL:
        this->lobView->receiveFromStartView(11, pack);
        break;
    case TYPE_EXIT_ROOM_SUCCESS_UPDATE_USR:
        this->lobView->receiveFromStartView(12, pack);
        break;
    case TYPE_EXIT_ROOM_SUCCESS_CLEAR_USR:
        this->lobView->receiveFromStartView(13, pack);
        break;
    case TYPE_CLOSE_ROOM_SUCCESS_FOR_ROOMER:
        this->lobView->receiveFromStartView(14, pack);
        break;
    case TYPE_CLOSE_ROOM_SUCCESS_FOR_USR:
        this->lobView->receiveFromStartView(15, pack);
        break;
    case TYPE_BACK_USR_INFO:
        this->lobView->receiveFromStartView(16, pack);
        break;
    case TYPE_RECHARGE_SUCCESS:
        this->lobView->receiveFromStartView(17, pack);
        break;
    case TYPE_RECHARGE_FAIL:
        this->lobView->receiveFromStartView(18, pack);
        break;
    case TYPE_FIND_PW_FAIL_NAME:
        this->findPwView->receiveFromStarView(1, pack);
        break;
    case TYPE_FIND_PW_FAIL_PHONE:
        this->findPwView->receiveFromStarView(2, pack);
        break;
    case TYPE_FIND_PW_SUCCESS:
        this->findPwView->receiveFromStarView(3, pack);
        break;
    case TYPE_JOIN_ROOM_FAIL:
        this->lobView->receiveFromStartView(19, pack);
        break;
    case TYPE_SENT_FLOWER_SUCCESS_ROOMER:
        this->lobView->receiveFromStartView(20, pack);
        break;
    case TYPE_SENT_FLOWER_SUCCESS_USR:
        this->lobView->receiveFromStartView(21, pack);
        break;
    default:
        break;
    }
    }
}

//登录
void StartWindow::on_btnLogin_clicked()
{
    if(ui->lineName->text().isEmpty()){
        QMessageBox::warning(this, "登录", "用户名不能为空");
        return;
    }

    if(ui->linePw->text().isEmpty()){
        QMessageBox::warning(this, "登录", "密码不能为空");
        return;
    }

    QByteArray qName = ui->lineName->text().toLocal8Bit();
    const char* cName = qName.data();

    QByteArray qPw = ui->linePw->text().toLocal8Bit();
    const char* cPw = qPw.data();
    ui->linePw->clear();

    pack_t packReg;

    packReg.type = TYPE_LOGIN;
    qstrcpy(packReg.name, cName);
    qstrcpy(packReg.pwd, cPw);

    this->clientSocket->write((const char*)& packReg, sizeof(packReg));
}

//找回密码
void StartWindow::on_btnFindPw_clicked()
{
    this->findPwView->setSocket(this->clientSocket);
    this->findPwView->show();
}
