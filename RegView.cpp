#include "RegView.h"
#include "ui_RegView.h"
#include <QMessageBox>
#include <QDebug>
#include <QHostAddress>

RegView::RegView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RegView)
{
    ui->setupUi(this);

    QWidget *p = this->parentWidget();

    this->clientSocket = new QTcpSocket(p);
}

RegView::~RegView()
{
    delete ui;
    delete this->clientSocket;
}

void RegView::showRegView(QTcpSocket *socket)
{
    this->setClient(socket);
    qDebug() << "套接字复制";

    this->show();
}

void RegView::receiveFromStartView(int type)
{
    switch(type){
    case 1:
        QMessageBox::information(this, "注册", "注册成功");
        break;
    case 2:
        QMessageBox::critical(this, "注册", "注册失败\n(用户名已存在)");
        break;
    default:
        break;
    }
}

//注册
void RegView::on_btnRegist_clicked()
{
    if(ui->lineName->text().isEmpty()){
        QMessageBox::warning(this, "注册", "用户名不能为空");
        return;
    }

    if(ui->linePw->text().isEmpty()){
        QMessageBox::warning(this, "注册", "密码不能为空");
        return;
    }

    if(ui->linePhone->text().isEmpty()){
        QMessageBox::warning(this, "注册", "密码提示不能为空");
        return;
    }

    QByteArray qName = ui->lineName->text().toLocal8Bit();
    const char* cName = qName.data();

    QByteArray qPw = ui->linePw->text().toLocal8Bit();
    const char* cPw = qPw.data();

    QByteArray qPhone = ui->linePhone->text().toLocal8Bit();
    const char* cPhone = qPhone.data();

    pack_t packReg;

    packReg.type = TYPE_REGIST;
    qstrcpy(packReg.name, cName);
    qstrcpy(packReg.pwd, cPw);
    qstrcpy(packReg.phone, cPhone);

    ui->lineName->clear();
    ui->linePhone->clear();
    ui->linePw->clear();

    this->clientSocket->write((const char*)& packReg, sizeof(packReg));
}

//返回
void RegView::on_btnBack_clicked()
{
    QWidget * p = this->parentWidget();
    p->show();
    this->hide();
}
