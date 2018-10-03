#include "playerwidget.h"
#include <QTimer>
//PlayerWidget::PlayerWidget(QWidget *parent) : QWidget(parent),src("rtsp://192.168.2.103:8554/live/test1")
PlayerWidget::PlayerWidget(QWidget *parent) : QWidget(parent),src("/root/test.mp4")
{
    tick_timer=new QTimer();
    connect(tick_timer,SIGNAL(timeout()),this,SLOT(timeout()));

    tick_timer->start(100);
}
