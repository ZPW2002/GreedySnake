#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFrame>
#include <QTimer>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>

#define Edge 30

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void Show();
    void Food();
    void Score();
    void keyPressEvent(QKeyEvent* event);


private slots:
    void Move();

private:
    bool first = true;
    Ui::Widget *ui;

    QTimer* runner = new QTimer();
    QFrame *map = new QFrame[Edge*Edge];

    QGridLayout* mainLayout = new QGridLayout(this);
    //QHBoxLayout * layout = new QHBoxLayout(this);
};
#endif // WIDGET_H
