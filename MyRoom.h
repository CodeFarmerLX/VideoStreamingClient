#ifndef MYROOM_H
#define MYROOM_H

#include <QMainWindow>
#include "package.h"
#include <QTcpSocket>
#include <QCamera>
#include "VideoSuface.h"
#include <QBuffer>
#include <QUdpSocket>
#include <QAudioInput>

namespace Ui {
class MyRoom;
}

class MyRoom : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyRoom(QWidget *parent = 0);
    ~MyRoom();

    void showMyRoomView(pack_t &pack,  QTcpSocket *socket);
    void setClient(QTcpSocket* socket){ this->clientSocket = socket; }
    void receiveFromLobbyView(int state, pack_t &pack);
    void clearUsrList();
    void clearMessageList();

    void setPort(unsigned short roomport) { this->port = roomport; }
    unsigned short getPort(){ return this->port; }

    void gift(const QString & text);


private slots:
    void on_btnBack_clicked();
    void frameChangedSlot(QVideoFrame);

    void on_btnOpenCamera_clicked();

    void on_btnCloseCamera_clicked();

    void on_btnVoiceOpen_clicked();

    void voiceReadSlot();

    void on_btnVoiceClose_clicked();

private:
    Ui::MyRoom *ui;
    QTcpSocket *clientSocket;
    QCamera* camera;
    VideoSuface* vs;
    QUdpSocket* writeSocket;
    unsigned short port;

    QUdpSocket* voiceSocket;
    QAudioInput *input;
    QIODevice *audioInputIODevice;

    struct voice
    {
        char data[1024];
        int lens;
    };

};

#endif // MYROOM_H
