#ifndef USRSLEEP_H
#define USRSLEEP_H

#include <QApplication>
#include <QTime>

void usrSleep(unsigned int msec);

void usrSleep(unsigned int msec){

    QTime reachTime =  QTime::currentTime().addMSecs(msec);

    while(QTime::currentTime() < reachTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

#endif // USRSLEEP_H
