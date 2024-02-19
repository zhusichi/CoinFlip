#include "mycoin.h"
#include<QDebug>
MyCoin::MyCoin(QWidget *parent) : QPushButton(parent)
{

}

MyCoin::MyCoin(QString btnImg)
{
    QPixmap pix;
    bool res= pix.load(btnImg);
    if(!res)
    {
        qDebug()<<"图片加载失败";
        return;
    }

    this->setFixedSize(pix.width(),pix.height());
    this->setStyleSheet("border:0px");
    this->setIcon(pix);
    this->setIconSize(QSize(pix.width(),pix.height()));

    timer1=new QTimer(this);
    timer2=new QTimer(this);

    connect(timer1,&QTimer::timeout,this,[=](){
        QPixmap pix;
        QString str=QString(":/res/Coin000%1.png").arg(this->min++);
        pix.load(str);

        this->setFixedSize(pix.width(),pix.height());
        this->setStyleSheet("border:0px");
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(),pix.height()));
        if(this->min > this->max)
        {
            this->min=1;
            this->isAnimation=false;
            timer1->stop();
        }
    });
    connect(timer2,&QTimer::timeout,this,[=](){
        QPixmap pix;
        QString str=QString(":/res/Coin000%1.png").arg(this->max--);
        pix.load(str);

        this->setFixedSize(pix.width(),pix.height());
        this->setStyleSheet("border:0px");
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(),pix.height()));
        if(this->min > this->max)
        {
            this->max=8;
            this->isAnimation=false;
            timer2->stop();
        }
    });
}

void MyCoin::mousePressEvent(QMouseEvent *e)
{
    if(this->isAnimation || this->isWin)
    {
        return;
    }
    else
    {
        QPushButton::mousePressEvent(e);
    }
}

void MyCoin::changeFlag()
{
    if(flag )
    {
        this->flag=false;
        this->isAnimation=true;
        timer1->start(30);
    }
    else
    {
        this->isAnimation=true;
        timer2->start(30);
        this->flag=true;
    }
}
