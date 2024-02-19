#include "mypushbutton.h"
#include<QDebug>
#include<QPropertyAnimation>
myPushButton::myPushButton(QWidget *parent) : QPushButton(parent)
{

}

myPushButton::myPushButton(QString normalImg,QString pressImg)
{
    this->normalImgPath=normalImg;
    this->pressImgPath=pressImg;

    QPixmap pix;
    bool ret=pix.load(normalImg);
    if(!ret)
    {
        qDebug()<<"图片加载失败";
        return;
    }

    this->setFixedSize(pix.width(),pix.height());

    this->setStyleSheet("border:0px");//QPushButton()

    this->setIcon(pix);

    this->setIconSize(QSize(pix.width(),pix.height()));
}

void myPushButton::zoom1()
{
    QPropertyAnimation* animation=new QPropertyAnimation(this,"geometry");

    animation->setDuration(200);
    animation->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));
    animation->setEndValue(QRect(this->x(),this->y()+10,this->width(),this->height()));

    animation->setEasingCurve(QEasingCurve::OutBounce);
    animation->start();
}
void myPushButton::zoom2()
{
    QPropertyAnimation* animation=new QPropertyAnimation(this,"geometry");

    animation->setDuration(200);
    animation->setStartValue(QRect(this->x(),this->y()+10,this->width(),this->height()));
    animation->setEndValue(QRect(this->x(),this->y(),this->width(),this->height()));

    animation->setEasingCurve(QEasingCurve::OutBounce);
    animation->start();
}

void myPushButton::mousePressEvent(QMouseEvent *e)
{
    if(this->pressImgPath!="")
    {
        QPixmap pix;
        bool res= pix.load(this->pressImgPath);
        if(!res)
        {
            qDebug()<<"图片加载失败";
            return;
        }

        this->setFixedSize(pix.width(),pix.height());

        this->setStyleSheet("border:0px");//QPushButton()

        this->setIcon(pix);

        this->setIconSize(QSize(pix.width(),pix.height()));
    }
    return QPushButton::mousePressEvent(e);
}
void myPushButton::mouseReleaseEvent(QMouseEvent *e)
{
    if(this->pressImgPath!="")
    {
        QPixmap pix;
        bool res= pix.load(this->normalImgPath);
        if(!res)
        {
            qDebug()<<"图片加载失败";
            return;
        }

        this->setFixedSize(pix.width(),pix.height());

        this->setStyleSheet("border:0px");//QPushButton()

        this->setIcon(pix);

        this->setIconSize(QSize(pix.width(),pix.height()));
    }
    return QPushButton::mouseReleaseEvent(e);
}
