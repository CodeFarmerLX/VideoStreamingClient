#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>
#include "package.h"
#include "LobbyView.h"
#include "RegView.h"
#include <QTcpSocket>
#include "FindPw.h"


namespace Ui {
class StartWindow;
}

class StartWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StartWindow(QWidget *parent = 0);
    ~StartWindow();

private slots:
    void on_btnRegist_clicked();

    void readyReadSlot();

    void on_btnLogin_clicked();

    void on_btnFindPw_clicked();

private:
    Ui::StartWindow *ui;
    LobbyView * lobView;
    RegView * regView;
    QTcpSocket * clientSocket;
    FindPw * findPwView;
};

#endif // STARTWINDOW_H
