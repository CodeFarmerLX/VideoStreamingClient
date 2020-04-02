#ifndef LOBBYVIEW_H
#define LOBBYVIEW_H

#include <QMainWindow>
#include "package.h"
#include <QTcpSocket>
#include "MyRoom.h"
#include "UsrRoom.h"

namespace Ui {
class LobbyView;
}

class LobbyView : public QMainWindow
{
    Q_OBJECT

public:
    explicit LobbyView(QWidget *parent = 0);
    ~LobbyView();

    void showLobbyView(pack_t & pack, QTcpSocket * socket);
    void setClient(QTcpSocket * socket) {this->clientSocket = socket; }
    void receiveFromStartView(int state, pack_t &pack);

    void setId(int id){ this->id = id; }
    void setName(const QString& name) { this->name = name; }

    int getId()const { return this->id; }
    QString getName() const { return this->name; }

private slots:
    void on_btnBack_clicked();

    void on_btnClose_clicked();

    void on_btnUpdate_clicked();

    void on_btnStartNewRoom_clicked();

    void on_listWidgetRoom_doubleClicked(const QModelIndex &index);

    void on_btnJoinRoom_clicked();

    void on_btnReMoney_clicked();

    void on_btnUpdateUsrInfo_clicked();

private:
    Ui::LobbyView *ui;
    MyRoom * myRoomView;
    UsrRoom * usrRoomView;

    QTcpSocket *clientSocket;

    int id;
    QString name;
};

#endif // LOBBYVIEW_H
