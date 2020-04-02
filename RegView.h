#ifndef REGVIEW_H
#define REGVIEW_H

#include <QMainWindow>
#include "package.h"
#include <QTcpSocket>

namespace Ui {
class RegView;
}

class RegView : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegView(QWidget *parent = 0);
    ~RegView();

    void showRegView(QTcpSocket * socket);

    void setClient(QTcpSocket* socket){ this->clientSocket = socket; }

    void receiveFromStartView(int type);

private slots:
//    void readyReadSlot();

    void on_btnRegist_clicked();

    void on_btnBack_clicked();

private:
    Ui::RegView *ui;
    QTcpSocket * clientSocket;
};

#endif // REGVIEW_H
