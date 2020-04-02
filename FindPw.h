#ifndef FINDPW_H
#define FINDPW_H

#include <QMainWindow>
#include <QTcpSocket>
#include "package.h"

namespace Ui {
class FindPw;
}

class FindPw : public QMainWindow
{
    Q_OBJECT

public:
    explicit FindPw(QWidget *parent = 0);
    ~FindPw();

    void setSocket(QTcpSocket * socket) { this->clientSocket = socket; }

    void receiveFromStarView(int state, pack_t & pack);

private slots:
    void on_btnBack_clicked();

    void on_btnFindPw_clicked();

private:
    Ui::FindPw *ui;
    QTcpSocket * clientSocket;
};

#endif // FINDPW_H
