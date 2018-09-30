#ifndef PVDMVNCPROCESSOR_H
#define PVDMVNCPROCESSOR_H

#include "videoprocessor.h"
//#include "movidiusprocessor.h"
class PvdMvncProcessor : public VideoProcessor
{
    typedef struct process_result{
        int width;
        int height;
        bool exist;
        int count;
        int front_count;
        int back_count;
        int other_count;
        int duration;
        int flow;
        vector <Rect> rects;
    }m_result;
    typedef struct args{
        double scale_ratio;
        int scan_step;
        Rect area;
        int no;
    }arg_t;
    arg_t arg;
public:
    PvdMvncProcessor()
    {

    }

    virtual bool process(Mat img_src,vector<Rect> &rects,Rect detect_area)
    {
      //  prt(info,"processor %x .......",this);
        MovidiusProcessor &pro= MovidiusProcessor::get_instance();
       // cout<<img_src.cols<<endl;
        prt(info,"processing frame (%d %d)",img_src.cols,img_src.rows);
        pro.process(img_src,rects);
        rects.push_back(Rect(555,222,333,444));
    }
};

///

#endif // PVDMVNCPROCESSOR_H
