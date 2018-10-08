#include "videosource.h"
#include <thread>
#include <functional>

VideoSource::VideoSource(string path):watch_dog(bind(&VideoSource::check_point,this)),
    frame_rate(0),is_pic(false),src_trd(NULL)
{
    only_key_frame=false;
    try_times=0;
    url=path;
    watch_dog.start(1000);
    vcap.open(path);
    prt(info,"start %s result: %d",path.data(),vcap.isOpened());

    quit_flg=false;
    //  thread(bind(&VideoSource::run,this)).detach();
    // _start_async(bind(&VideoSource::run,this));

    if(end_with_str(url,"png")){
        imread(url).copyTo(png_frame);
        prt(info,"read png");
        is_pic=true;
    }else
        src_trd=new thread(bind(&VideoSource::run,this));
}

VideoSource::VideoSource(string path,bool only_keyframe):watch_dog(bind(&VideoSource::check_point,this)),
    frame_rate(0),vcap(path),is_pic(false),src_trd(NULL)
{
    only_key_frame=only_keyframe;

    try_times=0;
    //  Timer1 t1(bind(&VideoSource::check_point,this));
    watch_dog.start(1000);
    prt(info,"%s",path.data());
    url=path;
    quit_flg=false;

    if(end_with_str(url,"png")){
        imread(url).copyTo(png_frame);
        prt(info,"read png");
        is_pic=true;
    }else
        src_trd=new thread(bind(&VideoSource::run,this));
}
VideoSource::~VideoSource()
{
    prt(info,"quiting video %s", url.data());
    quit_flg=true;
    if(src_trd){
        if(src_trd->joinable())
            src_trd->join();
        delete src_trd;
        prt(info,"quiting video thread %s", url.data());
    }
    watch_dog.stop();
    prt(info,"quit video: %s done", url.data());
}
void VideoSource::run()
{
#ifdef USE_CVCAP
    vcap=VideoCapture(url);
#else
    vcap=FfmpegVideoCapture(url);
#endif
    //  this_thread::sleep_for(chrono::milliseconds(1000));
    if(!vcap.isOpened()){
        prt(info,"fail to open %s", url.data());
    }else{
        prt(info,"ok to open %s", url.data());
    }

    if(1){
        frame_wait_time=0;
    }else{
        frame_wait_time=40;
    }

    Mat frame;
    int flag_retry=0;
    while(!quit_flg){
        if( vcap.isOpened()){
            flag_retry=0;
            frame.release();
            bool rt= vcap.read(frame);
            if(!rt){
                // cout<<url.data()<<" get frame error!"<<endl;
                prt(info,"get frame fail,restart video capture %s", url.data());
                vcap.release();
#ifdef USE_CVCAP
                vcap=VideoCapture(url);
#else
                vcap=FfmpegVideoCapture(url);
#endif
                //   vcap=VideoCapture( url.data());
                //vcap=PdVideoCapture( url.data());
            }
            if(frame.cols==0&&++try_times==100){
                vcap.release();
                prt(info,"%s get frame error,retrying ... ", url.data());
                continue;
            }else{
                // int ts=vcap.get(CV_CAP_PROP_POS_MSEC);
                //     int ts=vcap.get(CV_CAP_PROP_POS_FRAMES);;
                //    int ts=vcap.get(CV_CAP_PROP_FRAME_COUNT);

              //  int ts=vcap.get(CV_CAP_PROP_POS_AVI_RATIO);
//                double ts1=vcap.get(CV_CAP_PROP_POS_FRAMES);
//                double ts2=vcap.get(CV_CAP_PROP_FRAME_COUNT);
//                double ts3=vcap.get(CV_CAP_PROP_POS_MSEC);
               long  ts=cvGetTickCount();
               // prt(info,"%ld ", tc);
                               //prt(info,"timestamp  %dms", ts);
                frame_rate++;
                lock.lock();
                if(!(frame_rate%30))
                {
                    //prt(info,"running %s",url.data());
                }
                if(frame_list.size()<3&&frame.rows>0&&frame.cols>0){
                    frame_list.push_back(frame);
                    cur_ms_list.push_back(ts/1000000);
                }
                lock.unlock();
                if(frame_wait_time)
                    this_thread::sleep_for(chrono::milliseconds( frame_wait_time));
            }
        }else{
            if(flag_retry++<10){
                this_thread::sleep_for(chrono::milliseconds(100));
            }else{
                this_thread::sleep_for(chrono::seconds(1));
            }
#ifdef USE_CVCAP
            vcap=VideoCapture(url);
#else
            vcap=FfmpegVideoCapture(url);
#endif
            prt(info,"open url err:%s",url.data());
        }
    }
    prt(info,"thread is quiting");
    if( vcap.isOpened())
        vcap.release();
}

