#include <QPainter>
#include <time.h>
#include <QTimer>
#include <QString>
#include <QMessageBox>
#include "widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    //初始化场景界面尺寸
    resize(MARGIN*4+(AREA_COL+3)*BLOCK_SIZE,MARGIN*2+AREA_ROW*BLOCK_SIZE);
    //初始化游戏
    InitGame();
}

Widget::~Widget()
{

}

void Widget::InitGame()
{
    //初始化贪吃蛇,初始长度5,注意头部在前，尾部在后
    for(int j=4;j>=0;j--)
        snake.push_back(QPoint(j,0));
    dir=RIGHT;//初始时往右走
    //初始化果实
    srand(time(0));
    GenerateFood();
    //初始化游戏分数
    score=0;
    //初始化暂停变量
    isPause=false;
    //初始化计时器
    gameTimer=new QTimer(this);
    connect(gameTimer,SIGNAL(timeout()),this,SLOT(SnakeUpdate()));
    gameTimer->start(TIME_INTERVAL);
}

void Widget::GenerateFood()
{
    //随机产生位置
    foodPoint.setX(rand()%AREA_COL);
    foodPoint.setY(rand()%AREA_ROW);
    //如果与贪吃蛇位置冲突，重新生成
    if(snake.contains(foodPoint))
        GenerateFood();
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    //绘制游戏场景
    painter.setBrush(Qt::yellow);
    painter.setPen(Qt::blue);
    painter.drawRect(MARGIN,MARGIN,AREA_COL*BLOCK_SIZE,AREA_ROW*BLOCK_SIZE);
    //绘制贪吃蛇
    painter.setBrush(Qt::red);
    painter.setPen(Qt::green);
    for(int i=0;i<snake.size();i++)
        painter.drawRect(MARGIN+snake[i].x()*BLOCK_SIZE,MARGIN+snake[i].y()*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE);
    //绘制果实
    painter.setBrush(Qt::green);

    painter.drawEllipse(MARGIN+foodPoint.x()*BLOCK_SIZE,MARGIN+foodPoint.y()*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE);
    //绘制游戏分数
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial",14));
    painter.drawText(MARGIN*3+AREA_COL*BLOCK_SIZE,MARGIN+2*BLOCK_SIZE,"score: "+QString::number(score));
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    //贪吃蛇的方向是个状态机,注意各个方向之间切换有限制
    switch(event->key())
    {
    case Qt::Key_Up:
        if(dir!=DOWN)
            dir=UP;
        break;
    case Qt::Key_Down:
        if(dir!=UP)
            dir=DOWN;
        break;
    case Qt::Key_Left:
        if(dir!=RIGHT)
            dir=LEFT;
        break;
    case Qt::Key_Right:
        if(dir!=LEFT)
            dir=RIGHT;
        break;
    case Qt::Key_P:
        PauseResumeGame();
        break;
    default:
        break;
    }
}

bool Widget::IsGameOver()
{
    int x=snake.front().x();
    int y=snake.front().y();
    //出边界
    if(x<0||x>AREA_COL-1||y<0||y>AREA_ROW-1)
        return true;
    //撞了自己
    for(int i=3;i<snake.size();i++)
        if(snake[i]==snake.front())
            return true;
    return false;
}

void Widget::SnakeUpdate()
{
    //贪吃蛇移动的策略是每次删除尾部，然后添加新的头部，维护一个动态链表
    switch(dir)
    {
    case UP:
        snake.push_front(QPoint(snake.front().x(),snake.front().y()-1));
        break;
    case DOWN:
        snake.push_front(QPoint(snake.front().x(),snake.front().y()+1));
        break;
    case LEFT:
        snake.push_front(QPoint(snake.front().x()-1,snake.front().y()));
        break;
    case RIGHT:
        snake.push_front(QPoint(snake.front().x()+1,snake.front().y()));
        break;
    default:
        break;
    }
    //如果吃到了果实，则尾部不删除，否则删除尾部更新头部
    if(snake.contains(foodPoint))
    {
        score+=1; //得分
        GenerateFood(); //重新生成果实
    }
    else
        snake.pop_back();
    //游戏是否结束
    if(IsGameOver())
    {
        GameOver();
        return; //赶在重绘之前跳出函数，防止贪吃蛇真的出界
    }
    update(); //重绘，比repaint函数效果好
}

void Widget::GameOver()
{
    gameTimer->stop();
    QMessageBox::information(this,"failed","game over!");
}

void Widget::PauseResumeGame()
{
    //暂停和恢复定时器
    if(!isPause)
    {
        isPause=!isPause;
        gameTimer->stop();
    }
    else
    {
        isPause=!isPause;
        gameTimer->start(TIME_INTERVAL);
    }
}
