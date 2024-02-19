#include "playscene.h"
#include<QMenuBar>
#include<QPainter>
#include<QTimer>
#include<QDebug>
#include<QSound>
#include<QLabel>
#include<QPropertyAnimation>
#include"mypushbutton.h"
#include"mycoin.h"
#include"dataconfig.h"
PlayScene::PlayScene(QWidget *parent) : QMainWindow(parent)
{

}
PlayScene::PlayScene(int levelNum)
{
    this->levelIndex=levelNum;

    this->setFixedSize(380,588);
    this->setWindowIcon(QIcon(":/res/Coin0001.png"));
    this->setWindowTitle("游戏场景");

    QMenuBar* menu=menuBar();
    setMenuBar(menu);

    QMenu* open= menu->addMenu("开始");
    QAction* exit= open->addAction("退出");
    connect(exit,&QAction::triggered,this,[=](){
        this->close();
    });

    QSound *backSound=new QSound(":/res/BackButtonSound.wav",this);
    QSound *flipSound=new QSound(":/res/ConFlipSound.wav",this);
    QSound *winSound=new QSound(":/res/LevelWinSound.wav",this);

    myPushButton* backbtn=new myPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    backbtn->setParent(this);
    backbtn->move(this->width()-backbtn->width(),this->height()-backbtn->height());

    connect(backbtn,&myPushButton::clicked,this,[=](){
        backSound->play();

        QTimer::singleShot(400,this,[=](){
            emit this->chooseSceneBack();
        });
    });

    QLabel* label=new QLabel(this);
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(20);
    label->setFont(font);
    QString str1=QString("level：%1").arg(this->levelIndex);
    label->setText(str1);
    label->setGeometry(30,this->height()-50,180,50);

    dataConfig config;
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            this->gameArray[i][j]=config.mData[this->levelIndex][i][j];
        }
    }

    QLabel* winlabel=new QLabel;
    QPixmap tmppix;
    tmppix.load(":/res/LevelCompletedDialogBg.png");
    winlabel->setGeometry(0,0,tmppix.width(),tmppix.height());
    winlabel->setPixmap(tmppix);
    winlabel->setParent(this);
    winlabel->move(this->width()-tmppix.width()*1.25,-tmppix.height());//*0.5

    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            QLabel* label=new QLabel(this);
            label->setGeometry(0,0,50,50);
            label->setPixmap(QPixmap(":/res/BoardNode.png"));
            label->move(92+i*50,200+j*50);

            QString str;
            if(this->gameArray[i][j]==1)
            {
                str=":/res/Coin0001.png";
            }
            else
            {
                str=":/res/Coin0008.png";
            }
            MyCoin* coin=new MyCoin(str);
            coin->setParent(this);
            coin->move(94+i*50,203+j*50);

            coin->posX=i;
            coin->posY=j;
            coin->flag=gameArray[i][j];
            coinBtn[i][j]=coin;

            connect(coin,&MyCoin::clicked,this,[=](){
                flipSound->play();
                for(int i=0;i<4;i++)
                {
                    for(int j=0;j<4;j++)
                    {
                        this->coinBtn[i][j]->isWin=true;
                    }
                }
            });

            connect(coin,&MyCoin::clicked,this,[=](){
                coin->changeFlag();
                this->gameArray[i][j]=this->gameArray[i][j]==0?1:0;
                QTimer::singleShot(300,this,[=](){
                    if(coin->posX+1<4)
                    {
                        coinBtn[coin->posX+1][coin->posY]->changeFlag();
                        this->gameArray[coin->posX+1][coin->posY]=
                                this->gameArray[coin->posX+1][coin->posY]==0?1:0;
                    }
                    if(coin->posX-1>=0)
                    {
                        coinBtn[coin->posX-1][coin->posY]->changeFlag();
                        this->gameArray[coin->posX-1][coin->posY]=
                                this->gameArray[coin->posX-1][coin->posY]==0?1:0;
                    }
                    if(coin->posY+1<4)
                    {
                        coinBtn[coin->posX][coin->posY+1]->changeFlag();
                        this->gameArray[coin->posX][coin->posY+1]=
                                this->gameArray[coin->posX][coin->posY+1]==0?1:0;
                    }
                    if(coin->posY-1>=0)
                    {
                        coinBtn[coin->posX][coin->posY-1]->changeFlag();
                        this->gameArray[coin->posX][coin->posY-1]=
                                this->gameArray[coin->posX][coin->posY-1]==0?1:0;
                    }

                    for(int i=0;i<4;i++)
                    {
                        for(int j=0;j<4;j++)
                        {
                            this->coinBtn[i][j]->isWin=false;
                        }
                    }

                    this->isWin=true;
                    for(int i=0;i<4;i++)
                    {
                        for(int j=0;j<4;j++)
                        {
                            if(coinBtn[i][j]->flag==false)
                            {
                                this->isWin=false;
                                break;
                            }
                        }
                    }
                    if(this->isWin==true)
                    {
                        winSound->play();
                        for(int i=0;i<4;i++)
                        {
                            for(int j=0;j<4;j++)
                            {
                                coinBtn[i][j]->isWin=true;
                            }
                        }
                        QPropertyAnimation* animation=new QPropertyAnimation(winlabel,"geometry");

                        animation->setDuration(500);
                        animation->setStartValue(QRect(winlabel->x(),winlabel->y(),
                                                       winlabel->width(),winlabel->height()));
                        animation->setEndValue(QRect(winlabel->x(),winlabel->y()+114,
                                                     winlabel->winId(),winlabel->height()));
                        animation->setEasingCurve(QEasingCurve::OutBounce);
                        animation->start();
                    }

                });

            });
        }
    }
}

void PlayScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    pix.load(":/res/Title.png");
    pix=pix.scaled(pix.width()*0.5,pix.height()*0.5);
    painter.drawPixmap(10,30,pix.width(),pix.height(),pix);
}
