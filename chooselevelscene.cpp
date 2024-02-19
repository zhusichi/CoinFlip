#include "chooselevelscene.h"
#include<QMenuBar>
#include<QPainter>
#include<QTimer>
#include<QLabel>
#include<QDebug>
#include<QSound>
#include"mypushbutton.h"
ChooseLevelScene::ChooseLevelScene(QWidget *parent) : QMainWindow(parent)
{
    this->setFixedSize(380,588);
    this->setWindowIcon(QIcon(":/res/Coin0001.png"));
    this->setWindowTitle("选择关卡场景");

    QMenuBar* menu=menuBar();
    setMenuBar(menu);

    QMenu* startmenu= menu->addMenu("开始");
    QAction* exitmenu=startmenu->addAction("退出");//

    connect(exitmenu,&QAction::triggered,this,[=](){
        this->close();
    });

    QSound *chooseSound=new QSound(":/res/TapButtonSound.wav",this);
    QSound *backSound=new QSound(":/res/BackButtonSound.wav",this);

    myPushButton* backbtn=new myPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    backbtn->setParent(this);
    backbtn->move(this->width()-backbtn->width(),this->height()-backbtn->height());

    connect(backbtn,&QPushButton::clicked,this,[=](){

        backSound->play();
        QTimer::singleShot(400,this,[=](){
            emit this->chooseSceneBack();
        });
    });

    for(int i=0;i<20;i++)
    {
        myPushButton* menubtn=new myPushButton(":/res/LevelIcon.png");
        menubtn->setParent(this);
        menubtn->move(55+i%4*70,130+i/4*70);

        connect(menubtn,&myPushButton::clicked,this,[=](){
            chooseSound->play();

            this->hide();
            play=new PlayScene(i+1);
            play->setGeometry(this->geometry());
            play->show();

            connect(play,&PlayScene::chooseSceneBack,this,[=](){
                this->setGeometry(play->geometry());
                this->show();
                delete play;
                play=nullptr;
            });
        });

        QLabel* lable=new QLabel(this);
        lable->setFixedSize(menubtn->width(),menu->height());
        lable->setText(QString::number(i+1));
        lable->move(55+i%4*70,142+i/4*70);
        lable->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

        lable->setAttribute(Qt::WA_TransparentForMouseEvents);//51号事件
    }
}
void ChooseLevelScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/OtherSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);


    pix.load(":/res/Title.png");
    painter.drawPixmap((this->width()-pix.width())*0.5,30,pix.width(),pix.height(),pix);
}
