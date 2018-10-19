#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H

#include <QWidget>
#include "videosource.h"
#include <qmutex.h>
#include <QPainter>
#include <QMouseEvent>
#include "opencv2/opencv.hpp"
#include "camera_data.h"
#include <QMenu>
#include <QAction>

class MyAction:public QAction
{
    Q_OBJECT
public:
    MyAction(QWidget *w):QAction(w)
    {
        connect(this,SIGNAL(triggered(bool)),this,SLOT(trig(bool)));
    }

    MyAction(RequestPkt pk,string text,QWidget *w):QAction(w)
    {
        connect(this,SIGNAL(triggered(bool)),this,SLOT(trig(bool)));

        // connect(this,SIGNAL(()),this,SLOT(dest(QObject *)));

        txt=text;
        pkt=pk;
        this->setText(QString(txt.data()));
    }

signals:
    void choose(MyAction *p);

public slots:
    void dest(QObject *)
    {
        prt(info,"des %s",txt.data());
    }
    void trig(bool )
    {
        emit choose(this);
    }
public:
    RequestPkt pkt;
    string txt;
    bool checked;


};
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
    inline QImage get_test_img()
    {
        cv::Mat bgr_frame;
        cv::Mat rgb_frame;
        int ts;
        bgr_frame =cv::imread("res/no_image.png");
        //prt(info,"get ts %d",ts);
        if(!bgr_frame.empty()){
            cv::cvtColor(bgr_frame,rgb_frame,CV_BGR2RGB);
            cv::putText(rgb_frame,string("opening"),cv::Point(100,100),1,2,cv::Scalar(3),5);
            img=QImage((const uchar*)(rgb_frame.data),
                       rgb_frame.cols,rgb_frame.rows,
                       QImage::Format_RGB888);
            img.bits();
        }else{
            throw exception();
        }
        return img;
    }
    void set_delay(int frames)
    {
        delay_frames=frames;
        src.set_buffer_size(frames);
    }

protected:
    void paintEvent(QPaintEvent *)
    {
#if 1
        lock.lock();
        try{
            QPainter this_painter(this);
            if(!get_img()){
                img=get_test_img();
            }
            QPainter img_painter(&img);
            current_painter=&img_painter;
            if(show_input){
                // draw input
                camera_data.draw(bind(&PlayerWidget::draw_line,
                                      this,placeholders::_1,
                                      placeholders::_2,placeholders::_3,placeholders::_4),
                                 bind(&PlayerWidget::draw_circle,
                                      this,placeholders::_1,
                                      placeholders::_2,placeholders::_3,placeholders::_4),
                                 bind(&PlayerWidget::draw_text,
                                      this,placeholders::_1,
                                      placeholders::_2,placeholders::_3,placeholders::_4,
                                      placeholders::_5)
                                 );
            }
           // if(0){
            if(show_output){
                // draw output
                if(camera_data.data().str().size()>10)//TODO:better way?
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
                                     placeholders::_2,placeholders::_3,placeholders::_4,
                                     placeholders::_5)

                                );
            }

#if 0
            draw_text(QString("img ts: ").append(QString::number(timestamp)).toStdString().data(),VdPoint(200,320),100,PaintableData::Red,30);
            draw_text(QString("data ts late for : ").append(QString::number(timestamp-output_data.Timestamp)).append(" ms").toStdString().data(),VdPoint(200,350),100,PaintableData::Red,30);
#endif
            if(!img.isNull()){
                this_painter.drawImage(QRect(0,0,this->width(),this->height()),img);
            }
        }
        catch(exception e){
            prt(info,"exception in image");
        }

        lock.unlock();
#endif
    }
    inline void draw_line(VdPoint s,VdPoint e,int colour,int size)
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
    inline void draw_circle(VdPoint center,int rad,int colour,int size)
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
    inline void draw_text(string text,VdPoint center,int rad,int colour,int size)
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
        current_painter->drawText(center.x,center.y,QString(text.data()));
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
    void choose_method(bool )
    {
        prt(info,"choose method");

    }    void choose_method1( )
    {
        prt(info,"choose method ss  ");

    }
    void choose_item (MyAction *act )
    {
        // if(act->checked){
        prt(info,"%s select",act->text().toStdString().data());
        emit camera_request(act->pkt,this);
        //}
        //  clear_menu();
    }
    void clear_menu()
    {
        prt(info,"clear menu");
        for(QAction *a:actions){
            menu.removeAction(a);
            delete a;
        }
        actions.clear();
    }

    void mousePressEvent(QMouseEvent *e)
    {
        prt(info,"mouse press");
        if(e->button()==Qt::RightButton){
            vector <RequestPkt> reqs;
            vector <string> texts;
            camera_data.right_press(QPoint_2_VdPoint(map_point(e->pos())),reqs,texts);

            if(reqs.size()>0&&reqs.size()==texts.size()){
                for(int i=0;i<reqs.size();i++){
                    MyAction *ma=new MyAction(reqs[i],texts[i],this);
                    connect(ma,&MyAction::choose,this,&PlayerWidget::choose_item,Qt::DirectConnection);

                    actions.push_back(ma);
                    menu.addAction(ma);

                }

            }
            menu.exec(QCursor::pos());

        }else{
            camera_data.press(QPoint_2_VdPoint((map_point(e->pos()))));
        }
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
        // emit double_click_event(this);//TODO:set full screen?
    }


    void show_input_data(bool show)
    {
        if(show){
            show_input=true;
        }else{
            show_input=false;
        }
    }
    void show_output_data(bool show)
    {
        if(show){
            show_output=true;
        }else{
            show_output=false;
        }
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
        int ts;
        //   bool ret=src.get_frame(bgr_frame);
        bool ret=src.get_frame(bgr_frame,ts);
        timestamp=ts;
        //prt(info,"get ts %d",ts);
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
    int timestamp;
    QTimer *tick_timer;
    CameraInputData camera_data;
    CameraOutputData output_data;

    QPainter *current_painter;
    vector< MyAction *> actions;
    QMenu menu;
    bool show_input;
    bool show_output;
    int delay_frames;
    //    QMenu player_menu;
    //    QAction choose_fvd;
};

#endif // PLAYERWIDGET_H
