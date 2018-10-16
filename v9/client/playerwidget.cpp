#include "playerwidget.h"
#include <QTimer>
//PlayerWidget::PlayerWidget(QWidget *parent) : QWidget(parent),src("rtsp://192.168.2.103:8554/live/test1")
PlayerWidget::PlayerWidget(CameraInputData data,QWidget *parent) : QWidget(parent),src(data.Url)
  //PlayerWidget::PlayerWidget(CameraInputData data,QWidget *parent) : QWidget(parent),src("/root/test.mp4")
{
    camera_data=data;
    tick_timer=new QTimer();
    connect(tick_timer,SIGNAL(timeout()),this,SLOT(timeout()));
    connect(&menu,SIGNAL(aboutToHide()),this,SLOT(clear_menu()),Qt::QueuedConnection);
    tick_timer->start(100);//check frame aviliable every 100ms
}
