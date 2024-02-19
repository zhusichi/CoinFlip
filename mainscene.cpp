#include "mainscene.h"
#include "ui_mainscene.h"
#include<QPainter>
#include"mypushbutton.h"
#include<QDebug>
#include<QTimer>
#include<QSound>
MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainScene)
{
    ui->setupUi(this);
    setWindowTitle("老帮主带带你");
    setFixedSize(380,588);
    setWindowIcon(QIcon(":/res/Coin0001.png"));
    connect(ui->actionexit,&QAction::triggered,this,[=](){
        this->close();
    });

    QSound* startSound=new QSound(":/res/TapButtonSound.wav",this);



    myPushButton* startbtn=new myPushButton(":/res/MenuSceneStartButton.png");
    startbtn->setParent(this);

    startbtn->move(this->width()*0.5-startbtn->width()*0.5,this->height()*0.7);

    chooseScene=new ChooseLevelScene;

    connect(chooseScene,&ChooseLevelScene::chooseSceneBack,this,[=](){
        this->setGeometry(chooseScene->geometry());
        chooseScene->hide();
        this->show();
    });
    connect(startbtn,&QPushButton::clicked,this,[=](){
        startSound->play();
        startbtn->zoom1();
        startbtn->zoom2();

        QTimer::singleShot(400,this,[=](){
            chooseScene->setGeometry(this->geometry());
            this->hide();
            chooseScene->show();
        });

    });
}

void MainScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    pix.load(":/res/Title.png");
    pix= pix.scaled(pix.width()*0.5,pix.height()*0.5);
    painter.drawPixmap(10,30,pix);
}

MainScene::~MainScene()
{
    delete ui;
}

