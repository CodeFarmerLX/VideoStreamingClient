#ifndef MYBARRAGE_H
#define MYBARRAGE_H

#include <QWidget>
#include <QLabel>
#include <QTimer>

class Mybarrage : public QLabel
{
    Q_OBJECT

private:
    int x;//弹幕横坐标位置
    int y;

public:
    explicit Mybarrage(QWidget *parent = nullptr);
    Mybarrage(const QString& str,QWidget *parent = nullptr);//字幕
    Mybarrage(const QPixmap& image, QWidget *parent = nullptr);

    void myMove();

signals:

public slots:
    void onTimer();

};

#endif // MYBARRAGE_H
