#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H

#include <QWidget>
#include "videosource.h"
#include <qmutex.h>
#include <QPainter>
class PlayerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlayerWidget(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *)
    {
#if 1
        lock.lock();
        QPainter this_painter(this);
        if(!get_img()){
            // prt();
        }

        //        QPainter img_painter(&img);
        //        img_painter.setPen(blue_pen3());
        if(!img.isNull()){
            this_painter.drawImage(QRect(0,0,this->width(),this->height()),img);
        }
        lock.unlock();
#endif
    }
    void  initializeGL()
    {

    }
signals:

public slots:
    void timeout()
    {
        this->update();
    }
private:
    bool  get_img()
    {
        Mat rgb_frame;
        Mat bgr_frame;
        bool ret=src.get_frame(bgr_frame);
        if(ret){
            cvtColor(bgr_frame,rgb_frame,CV_BGR2RGB);
            img=QImage((const uchar*)(rgb_frame.data),
                       rgb_frame.cols,rgb_frame.rows,
                       QImage::Format_RGB888);
            img.bits();
        }
        return ret;
    }
    VideoSource src;
    QMutex lock;
    QImage img;
    QTimer *tick_timer;
};

#endif // PLAYERWIDGET_H
