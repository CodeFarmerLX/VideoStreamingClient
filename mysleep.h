#ifndef MYSLEEP_H
#define MYSLEEP_H

#include <QApplication>
#include <QTime>

void mySleep(unsigned int msec);

void mySleep(unsigned int msec){

    QTime reachTime =  QTime::currentTime().addMSecs(msec);

    while(QTime::currentTime() < reachTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}
#endif // MYSLEEP_H
