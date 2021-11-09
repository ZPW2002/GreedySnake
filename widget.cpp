#include "widget.h"
#include "ui_widget.h"
#include "snake.h"
#include <QPixmap>
#include <QSound>
#include <QMessageBox>

static int score[2] = {0,0};
static int food;
static int speed=100;
static int length = 8;
static int Map[Edge][Edge];
int xDir[4]={1,0,-1,0},yDir[4]={0,-1,0,1};
static Snake snake[8];
static int number[10][21] = {
    {18,780,810,840,870,871,872,873,874,875,876,846,816,786,785,784,783,782,781},
    {7,870,871,872,873,874,875,876},
    {16,780,810,840,870,871,872,873,843,813,783,784,785,786,816,846,876},
    {16,780,810,840,870,871,872,873,874,875,876,846,816,786,783,813,843},
    {13,780,781,782,783,813,843,873,872,871,870,874,875,876},
    {16,870,840,810,780,781,782,783,813,843,873,874,875,876,846,816,786},
    {18,780,810,840,870,873,874,875,876,846,816,786,785,784,783,782,781,813,843},
    {10,780,810,840,870,871,872,873,874,875,876},
    {20,780,810,840,870,871,872,873,874,875,876,846,816,786,785,784,783,782,781,813,843},
    {18,780,810,840,870,871,872,873,874,875,876,846,816,786,783,782,781,813,843}
};

QSound* sound = new QSound(":/sound/backgroundMusic.wav");

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    srand(time(0));

    ui->setupUi(this);

    resize(25*Edge,25*Edge);
    mainLayout->setSpacing(0);

    sound->setLoops(100);
    sound->play();

    for(int i=0; i<8; i++) snake[i].set(10, 7-i, 3);

    for(int i=0; i<Edge*Edge; i++)
    {
        int x = i%Edge, y = i/Edge;

        mainLayout->addWidget(&map[i], x, y);
        map[i].setAutoFillBackground(true);
        map[i].setMinimumSize(25,25);
        map[i].setMaximumSize(25,25);
        map[i].setStyleSheet("background-image: url(:/images/Background.png)");
    }

    connect(runner, SIGNAL(timeout()), this, SLOT(Move()));
    runner->start(speed);
}

void Widget::Move()
{
    Food();

    int last_x = snake[length-1].x;
    int last_y = snake[length-1].y;
    int last_dir = snake[length-1].dir;
    int tx = snake[0].x + xDir[snake[0].dir];
    int ty = snake[0].y + yDir[snake[0].dir];

    for(int i=length-1; i>0; i--)
        snake[i] = snake[i-1];

    //死亡判定
    if(!(!Map[tx][ty] || Map[tx][ty]==2) ||tx >= Edge||ty >= Edge||tx < 0||ty < 0)
    {
        runner->stop();
        sound->stop();
        QString text = "游戏结束\n得分: "+QString::number(score[0]*10+score[1]);
        QMessageBox::information(NULL, "通知", text);
        exit(0);
    }
    //得分判定
    if(Map[tx][ty]==2)
    {
        length++;
        food--;
        score[1]++;
        if(score[1]>=10)
        {
            score[0]++;
            score[1]-=10;
        }
        snake[length-1].set(last_x,last_y,last_dir);
        QSound::play(":/sound/get.wav");
    }
    else
    {
        Map[last_x][last_y]=0;
        map[last_x*Edge + last_y].setStyleSheet("background-image: url(:/images/Background.png)");
    }

    snake[0].set(tx,ty,snake[0].dir);

    for(int i=1;i<length;i++)
    {
        if(snake[i].dir!=snake[i-1].dir)
            Map[snake[i-1].x][snake[i-1].y]=(snake[i-1].dir+1)*10+snake[i].dir;
        else
        {
            if(snake[i].dir==1||snake[i].dir==3)
                Map[snake[i].x][snake[i].y] = 51;
            if(snake[i].dir==0||snake[i].dir==2)
                Map[snake[i].x][snake[i].y] = 52;
        }

    }
    Map[snake[0].x][snake[0].y]=100+snake[0].dir;
    Map[snake[length-1].x][snake[length-1].y]=105+snake[length-1].dir;

    Show();
    Score();
}

void Widget::Score()
{
    if(score[0])
        for(int i = 1;i<=number[score[0]-1][0];i++)
            map[number[score[0]-1][i]-150].setStyleSheet("background-image: url(:/images/Background.png)");
    else
        for(int i = 1;i<=number[9][0];i++)
            map[number[9][i]-150].setStyleSheet("background-image: url(:/images/Background.png)");

    if(score[1])
        for(int i = 1;i<=number[score[1]-1][0];i++)
            map[number[score[1]-1][i]].setStyleSheet("background-image: url(:/images/Background.png)");
    else
        for(int i = 1;i<=number[9][0];i++)
            map[number[9][i]].setStyleSheet("background-image: url(:/images/Background.png)");

    for(int i = 1;i<=number[score[0]][0];i++)
    {
        map[number[score[0]][i]-150].setStyleSheet("background-image: url(:/images/score.png)");
    }

    for(int i = 1;i<=number[score[1]][0];i++)
    {
        map[number[score[1]][i]].setStyleSheet("background-image: url(:/images/score.png)");
    }
}

void Widget::Show()
{
    int array[3]={0,1,length-1};
    int t = 3, j, t_map;
    if(first) t = length;

    for(int i=0;i<t;i++)
    {
        if(first)
        {
            j = snake[i].x*Edge+snake[i].y;
            t_map = Map[snake[i].x][snake[i].y];
        }
        else
        {
            j = snake[array[i]].x*Edge+snake[array[i]].y;
            t_map = Map[snake[array[i]].x][snake[array[i]].y];
        }

        switch (t_map) {
        case 20:
        case 33:
            map[j].setStyleSheet("background-image: url(:/images/body3.png)");
            break;
        case 40:
        case 31:
            map[j].setStyleSheet("background-image: url(:/images/body4.png)");
            break;
        case 11:
        case 42:
            map[j].setStyleSheet("background-image: url(:/images/body5.png)");
            break;
        case 13:
        case 22:
            map[j].setStyleSheet("background-image: url(:/images/body6.png)");
            break;

        case 51:
            map[j].setStyleSheet("background-image: url(:/images/body1.png)");
            break;
        case 52:
            map[j].setStyleSheet("background-image: url(:/images/body2.png)");
            break;
        case 100:
            map[j].setStyleSheet("background-image: url(:/images/head1.png)");
            break;
        case 101:
            map[j].setStyleSheet("background-image: url(:/images/head2.png)");
            break;
        case 102:
            map[j].setStyleSheet("background-image: url(:/images/head3.png)");
            break;
        case 103:
            map[j].setStyleSheet("background-image: url(:/images/head4.png)");
            break;
        case 105:
            map[j].setStyleSheet("background-image: url(:/images/tail1.png)");
            break;
        case 106:
            map[j].setStyleSheet("background-image: url(:/images/tail2.png)");
            break;
        case 107:
            map[j].setStyleSheet("background-image: url(:/images/tail3.png)");
            break;
        case 108:
            map[j].setStyleSheet("background-image: url(:/images/tail4.png)");
            break;

        default:
            break;
        }
    }
    first = false;
    return;
}

void Widget::Food()
{
    if(food>6)return;
Start:

    int i = rand()%(Edge*Edge);
    int x = i/Edge, y = i%Edge;
    if(Map[x][y])goto Start;

    food++;
    Map[x][y] = 2;
    map[i].setStyleSheet("background-image: url(:/images/food.png)");

}

void Widget::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Up && snake[0].dir!=3)
        snake[0].dir=1;
    if(event->key()==Qt::Key_Down && snake[0].dir!=1)
        snake[0].dir=3;
    if(event->key()==Qt::Key_Right && snake[0].dir!=2)
        snake[0].dir=0;
    if(event->key()==Qt::Key_Left && snake[0].dir!=0)
        snake[0].dir=2;
}

Widget::~Widget()
{
    delete ui;
    delete [] map;
}

