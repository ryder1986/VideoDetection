#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H

#include <QWidget>
#include "videosource.h"
#include <qmutex.h>
#include <QPainter>
#include <QMouseEvent>
#include "opencv2/opencv.hpp"
#include "camera_data.h"
class PlayerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlayerWidget(CameraInputData data,QWidget *parent = nullptr);
    void set_output_data(CameraOutputData data)
    {
        lock.lock();
        output_data=data;
        lock.unlock();
    }

protected:
    void paintEvent(QPaintEvent *)
    {
#if 1
        lock.lock();
        QPainter this_painter(this);
        if(!get_img()){
            // prt();
        }
        //   camera_data.change_url();
        //        QPainter img_painter(&img);
        //        img_painter.setPen(blue_pen3());
        QPainter img_painter(&img);
        current_painter=&img_painter;
        camera_data.draw(bind(&PlayerWidget::draw_line,
                              this,placeholders::_1,
                              placeholders::_2,placeholders::_3,placeholders::_4),
                         bind(&PlayerWidget::draw_circle,
                              this,placeholders::_1,
                              placeholders::_2,placeholders::_3,placeholders::_4));

        output_data.draw(
                         camera_data,
                         bind(&PlayerWidget::draw_line,
                              this,placeholders::_1,
                              placeholders::_2,placeholders::_3,placeholders::_4),
                         bind(&PlayerWidget::draw_circle,
                              this,placeholders::_1,
                              placeholders::_2,placeholders::_3,placeholders::_4),
                         bind(&PlayerWidget::draw_text,
                              this,placeholders::_1,
                              placeholders::_2,placeholders::_3,placeholders::_4)

                         );
        if(!img.isNull()){
            this_painter.drawImage(QRect(0,0,this->width(),this->height()),img);
        }
        lock.unlock();
#endif
    }
    void draw_line(VdPoint s,VdPoint e,int colour,int size)
    {

        switch (colour) {
        case PaintableData::Colour::Green:
            current_painter->setPen(QPen(QBrush(QColor(0,255,0)),size));
            break;
        case PaintableData::Colour::Red:
            current_painter->setPen(QPen(QBrush(QColor(255,0,0)),size));
            break;
        case PaintableData::Colour::Blue:
            current_painter->setPen(QPen(QBrush(QColor(0,0,255)),size));
            break;
        default:
            break;
        }

        QPen pen_ori=current_painter->pen();
        current_painter->drawLine(QPoint(s.x,s.y),QPoint(e.x,e.y));
        current_painter->setPen(pen_ori);
    }
    void draw_circle(VdPoint center,int rad,int colour,int size)
    {

        switch (colour) {
        case PaintableData::Colour::Green:
            current_painter->setPen(QPen(QBrush(QColor(0,255,0)),size));
            break;
        case PaintableData::Colour::Red:
            current_painter->setPen(QPen(QBrush(QColor(255,0,0)),size));
            break;
        case PaintableData::Colour::Blue:
            current_painter->setPen(QPen(QBrush(QColor(0,0,255)),size));
            break;
        default:
            break;
        }

        QPen pen_ori=current_painter->pen();
        current_painter->drawEllipse(QPoint(center.x,center.y),rad,rad);
        current_painter->setPen(pen_ori);
    }
    void draw_text(VdPoint center,int rad,int colour,int size)
    {

        switch (colour) {
        case PaintableData::Colour::Green:
            current_painter->setPen(QPen(QBrush(QColor(0,255,0)),size));
            break;
        case PaintableData::Colour::Red:
            current_painter->setPen(QPen(QBrush(QColor(255,0,0)),size));
            break;
        case PaintableData::Colour::Blue:
            current_painter->setPen(QPen(QBrush(QColor(0,0,255)),size));
            break;
        default:
            break;
        }

        QPen pen_ori=current_painter->pen();
        current_painter->drawEllipse(QPoint(center.x,center.y),rad,rad);
        current_painter->setPen(pen_ori);
    }
    void  initializeGL()
    {

    }


signals:
    void camera_request(RequestPkt req,PlayerWidget *w);
public slots:

    void timeout()
    {
        this->update();
    }

    void mousePressEvent(QMouseEvent *e)
    {
        prt(info,"mouse press");
        camera_data.press(QPoint_2_VdPoint((map_point(e->pos()))));
        //        vector <DetectRegionInputData >detect_regions;
        //        detect_regions.assign(cfg.DetectRegion.begin(),cfg.DetectRegion.end());

    }

    void mouseMoveEvent(QMouseEvent *e)
    {
        QPoint p1=map_point(e->pos());
        camera_data.move(QPoint_2_VdPoint((map_point(e->pos()))));
    }

    void mouseReleaseEvent(QMouseEvent *e)
    {
        prt(info,"mouse release");
         RequestPkt req1;
         if( camera_data.release(req1)){
                emit camera_request(req1,this);
         }
    }

    void mouseDoubleClickEvent(QMouseEvent *e)
    {



        //    if(++double_click_flag%2)
        //        emit click_event(this,ClickEvent::SHOW_ONE);
        //    else
        //        emit click_event(this,ClickEvent::SHOW_ALL);
    }
private:
    inline QPoint map_point(QPoint p)
    {
        return QPoint(p.x()*img.width()/this->width(),p.y()*img.height()/this->height());
    }
    inline VdPoint QPoint_2_VdPoint(QPoint p)
    {
        return VdPoint(p.x(),p.y());
    }
    bool  get_img()
    {
        cv::Mat rgb_frame;
        cv::Mat bgr_frame;
        bool ret=src.get_frame(bgr_frame);
        if(ret){
            cv::cvtColor(bgr_frame,rgb_frame,CV_BGR2RGB);
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
    CameraInputData camera_data;
    CameraOutputData output_data;

    QPainter *current_painter;
};

#endif // PLAYERWIDGET_H
