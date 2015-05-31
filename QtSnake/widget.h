#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QKeyEvent>

const int BLOCK_SIZE=25; //单个方块单元的边长
const int MARGIN=5; //场景边距
const int AREA_ROW=15; //场景行数
const int AREA_COL=15; //场景列数

const int TIME_INTERVAL=500; //定时器间隔时间

enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
    virtual void paintEvent(QPaintEvent *event); //界面刷新
    virtual void keyPressEvent(QKeyEvent *event); //键盘监听

public:
    void InitGame(); //初始化游戏
    void PauseResumeGame(); //暂停和恢复游戏
    void GameOver(); //游戏结束
    void GenerateFood(); //随机产生果实
    bool IsGameOver(); //判断是否游戏结束

private slots:
    void SnakeUpdate(); //贪吃蛇增长

private:
    QTimer *gameTimer; //游戏计时器
    bool isPause; //是否暂停
    QPoint foodPoint; //果实出现坐标
    QList<QPoint> snake; //贪吃蛇结构
    Direction dir; //方向
    int score; //分数
};

#endif // WIDGET_H
