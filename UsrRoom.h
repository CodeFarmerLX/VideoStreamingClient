#ifndef USRROOM_H
#define USRROOM_H

#include <QMainWindow>
#include "package.h"
#include <QTcpSocket>
#include <QUdpSocket>
#include <QAudioOutput>

namespace Ui {
class UsrRoom;
}

class UsrRoom : public QMainWindow
{
    Q_OBJECT

public:
    explicit UsrRoom(QWidget *parent = 0);
    ~UsrRoom();

    void showUsrRoom(pack_t &pack,  QTcpSocket *socket);
    void setClient(QTcpSocket* socket){ this->clientSocket = socket; }
    void receiveFromLobbyView(int state, pack_t &pack);
    void clearUsrList();
    void clearMessageList();

    void setRoomAddress(const QString & roomaddress){ this->roomAddress = roomaddress; }
    void setRoomPort(unsigned short roomport) { this->roomPort = roomport; }

    const QString& getRoomAddress(){ return this->roomAddress; }
    unsigned short getRoomPort(){ return this->roomPort; }


//    //弹幕
//    void barrage(const QString & text);

//    void setBarrage();

    void gift(const QString & text);


private slots:

    void on_btnSentText_clicked();

    void on_btnBack_clicked();

    void readyreadSlot();

//    void on_btnTest_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void voiceReadSlot();

private:
    Ui::UsrRoom *ui;
    QTcpSocket *clientSocket;
    QString roomAddress;
    unsigned short roomPort;
    QUdpSocket* readSocket;

    QUdpSocket* voiceSocket;
    QAudioOutput *output;
    QIODevice *audioOutputIODevice;

    struct voice
    {
        char data[1024];
        int lens;
    };

};

#endif // USRROOM_H
