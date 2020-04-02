#include "FindPw.h"
#include "ui_FindPw.h"
#include <QMessageBox>
#include  "package.h"

FindPw::FindPw(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FindPw)
{
    ui->setupUi(this);
}

FindPw::~FindPw()
{
    delete ui;
}

void FindPw::on_btnBack_clicked()
{
    this->close();
}

void FindPw::on_btnFindPw_clicked()
{
    if(ui->linePhone->text().isEmpty() || ui->lineName->text().isEmpty())
    {
        QMessageBox::critical(this, "找回密码", "提示信息或用户账号不能为空");
        return;
    }

    QString sphone = ui->linePhone->text();
    QByteArray bphone = sphone.toLocal8Bit();
    const char * cphone = bphone.data();

    QString sname = ui->lineName->text();
    QByteArray bname = sname.toLocal8Bit();
    const char * cname = bname.data();

    ui->lineName->clear();
    ui->linePhone->clear();

    pack_t packFindPw;
    memset(&packFindPw, 0, sizeof(packFindPw));

    packFindPw.type = TYPE_FIND_PW;
    qstrcpy(packFindPw.name, cname);
    qstrcpy(packFindPw.phone, cphone);

    this->clientSocket->write((const char *)&packFindPw, sizeof(packFindPw));
}

void FindPw::receiveFromStarView(int state, pack_t & pack)
{
    switch(state){
    case 1://账号不存在
        QMessageBox::critical(this, "找回密码", "账号不存在");
        break;
    case 2://密码提示不正确
        QMessageBox::critical(this, "找回密码", "密码提示不正确");
        break;
    case 3:{//找回密码成功
        QString pw = QString::fromLocal8Bit(pack.pwd);
        QMessageBox::information(this, "找回密码", "您的账号密码为:\n" + pw);
        break;
    }

    }
}
