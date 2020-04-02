#include "mybarrage.h"

Mybarrage::Mybarrage(QWidget *parent) : QLabel(parent)
{

}

Mybarrage::Mybarrage(const QString& str,QWidget *parent)
    :QLabel(parent)
{
    this->y = rand()%150 + 50;
    this->x = -20;//初始位置

    //设置字号
    QFont ft;
    ft.setPointSize(18);
    this->setFont(ft);
    //设置颜色
    QPalette pa;
    int r = 0, g = 0, b = 0;
    r = rand()%125 + 100;
    g = rand()%125 + 100;
    b = rand()%125 + 100;

    pa.setColor(QPalette::WindowText,QColor(r,g,b));

    this->setPalette(pa);

    this->setText(str);
}

void Mybarrage::myMove()
{
    QTimer *timer = new QTimer(this);//注意指定父对象
    connect(timer,SIGNAL(timeout()),
            this,SLOT(onTimer()));
    timer->start(50);//频率
}
void Mybarrage::onTimer()
{
    this->move(x,y);//150显示高度
    x += 4;               //步距
    if(x>= 400){          //终止位置
//        x = -20;           //重置初始位置
        delete this;
    }
}

Mybarrage::Mybarrage(const QPixmap& image, QWidget *parent)
    :QLabel(parent)
{
    this->y = rand()%150 + 200;
    this->x = -20;//初始位置
    this->setPixmap(image.scaled(150,35,Qt::IgnoreAspectRatio));
}
