#ifndef PLAYER_H
#define PLAYER_H

#include "videosource.h"

#include "tool.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/objdetect/objdetect.hpp>

using namespace std;
using namespace cv;

#ifdef __cplusplus
extern "C" {
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/frame.h>
#include <unistd.h>
#include <pthread.h>

#ifdef __cplusplus
}
#endif


class PdVideoCaptureVaapi{
public:
    PdVideoCaptureVaapi(string url):url(url)
    {
        opened=false;
        width=0;
        height=0;
        if(!init_input()){
            prt(info,"@@@@@err");
            //exit(0);
        }else{
            if(!init_decoder()){
                prt(info,"@@@@@err");
                //exit(0);
            }else{
                opened=true;
            }
        }

        //  int t= cvNamedWindow("123");
    }
    ~PdVideoCaptureVaapi()
    {

    }
    bool read(Mat &BGRImage)
    {
        // int t= cvNamedWindow("123");
        Mat YUVImage;
        int size;
        size=width*height;
        av_init_packet(&av_pkt);
        if ((av_read_frame(fmt, &av_pkt) < 0)) {
            prt(info,"read frm fail");
            return false;
        }
        //    fmt->iformat->read_header()
        //rtsp_read_header
        //  read_header()
        //    RTSPState *state = _formatCtx->priv_data;
        //        RTSPStream *stream = state->rtsp_streams[0];
        //        RTPDemuxContext *demux = stream->transport_priv;
        //        demux->timestamp

        //RTSPState *state=(RTSPState) fmt->priv_data;
        int64_t dt,wt;
        av_get_output_timestamp(fmt,0,&dt,&wt);
        //        prt(info,"read frm stamp %d ",fmt->start_time_realtime);
        //        prt(info,"read frm stamp1 %d ",fmt->start_time);
        //        prt(info,"read frm stamp2 %d ",av_pkt.pts);
        //        prt(info,"read frm stamp3 %d ",av_pkt.dts);
        //        prt(info,"read frm stamp4 %d ",av_pkt.pos);
        if(decode()){
            // prt(info,"decode a frame");
            YUVImage.create(width*3/2, height, CV_8UC1);
            memcpy(YUVImage.data, buf_y, size);
            memcpy(YUVImage.data + size, buf_u, size /4);
            memcpy(YUVImage.data + size + size /4, buf_v, size / 4);
            cvtColor(YUVImage, BGRImage, CV_YUV2BGR_I420);
            // imshow("123",BGRImage);
            //   waitKey(10);
            return true;
        }else{
            prt(info,"decode a fail");
            return false;
        }
    }
    bool isOpened()
    {
        return opened;
    }
    void release()
    {
        release_input();
        release_decoder();
        opened=false;
    }
    int get(int cmd)
    {
        int ret;
        switch(cmd){
        case CV_CAP_PROP_FRAME_WIDTH:ret=width;break;
        case CV_CAP_PROP_FRAME_HEIGHT:ret=height;break;
        default:break;

        }
        return ret;
    }
private:
    bool init_input()
    {
        bool ret=true;
        av_register_all();
        avformat_network_init();
        fmt = avformat_alloc_context();
        if(avformat_open_input(&fmt, url.data(), NULL, 0) != 0) {
            ret=false;
        } else if (avformat_find_stream_info(fmt, NULL) < 0) {
            ret=false;
        }
        return ret;
    }
    bool init_decoder()
    {
        bool ret=false;
        //codec = avcodec_find_decoder(CODEC_ID_H264);
        codec = avcodec_find_decoder(AV_CODEC_ID_H264);
        codec_ctx = avcodec_alloc_context3(codec);
        if (avcodec_open2(codec_ctx, codec, NULL) >= 0){
            avframe=NULL;
            // avframe= avcodec_alloc_frame();av_frame_alloc();
            avframe= av_frame_alloc();
            if(avframe)
                ret=true;
        }
        else {
            prt(info,"fail to open decoder");
        }
        return ret;
    }
    void release_decoder()
    {
        av_free(avframe);
        avcodec_close(codec_ctx);
    }
    void release_input()
    {
        avformat_close_input(&fmt);
    }

    bool decode()
    {
        int got_picture = 0;
        int len = 0;

        while (av_pkt.size > 0) {
            // prt(info,"decoing size: %d",av_pkt.size);
            len = avcodec_decode_video2(codec_ctx, avframe, &got_picture, &av_pkt);
            if (len < 0) {
                return false;
            }
            if (got_picture)
            {
                buf_y = (unsigned char *) avframe->data[0];
                buf_u = (unsigned char *) avframe->data[1];
                buf_v = (unsigned char *) avframe->data[2];
                //      prt(info,"decode size %d  (%d  %d)",av_pkt.size,avframe->width,avframe->height);
                //                prt(info,"%d",av_pkt.pts);
                width=avframe->height;
                height=avframe->width;


                return true;

            }
            av_pkt.size -= len;
            av_pkt.data += len;
        }
        return false;
    }


public:
    AVCodec *codec;
    AVCodecContext *codec_ctx;
    AVFrame *avframe;
    AVFormatContext *fmt;
    AVPacket av_pkt;
    bool opened;
    string url;
    unsigned char *buf_y;
    unsigned char *buf_u;
    unsigned char *buf_v;
    int width;
    int height;
};

class PdVideoCaptureSdk{
public:
    PdVideoCaptureSdk(string url):url(url)
    {
        opened=false;
        width=0;
        height=0;
        if(!init_input()){
            prt(info,"@@@@@err");
            //exit(0);
        }else{
            if(!init_decoder()){
                prt(info,"@@@@@err");
                //exit(0);
            }else{
                opened=true;
            }
        }

        //  int t= cvNamedWindow("123");
    }
    ~PdVideoCaptureSdk()
    {

    }
    bool read(Mat &BGRImage)
    {
        // int t= cvNamedWindow("123");
        Mat YUVImage;
        int size;
        size=width*height;
        av_init_packet(&av_pkt);
        if ((av_read_frame(fmt, &av_pkt) < 0)) {
            prt(info,"read frm fail");
            return false;
        }
        //    fmt->iformat->read_header()
        //rtsp_read_header
        //  read_header()
        //    RTSPState *state = _formatCtx->priv_data;
        //        RTSPStream *stream = state->rtsp_streams[0];
        //        RTPDemuxContext *demux = stream->transport_priv;
        //        demux->timestamp

        //RTSPState *state=(RTSPState) fmt->priv_data;
        int64_t dt,wt;
        av_get_output_timestamp(fmt,0,&dt,&wt);
        //        prt(info,"read frm stamp %d ",fmt->start_time_realtime);
        //        prt(info,"read frm stamp1 %d ",fmt->start_time);
        //        prt(info,"read frm stamp2 %d ",av_pkt.pts);
        //        prt(info,"read frm stamp3 %d ",av_pkt.dts);
        //        prt(info,"read frm stamp4 %d ",av_pkt.pos);
        if(decode()){
            // prt(info,"decode a frame");
            YUVImage.create(width*3/2, height, CV_8UC1);
            memcpy(YUVImage.data, buf_y, size);
            memcpy(YUVImage.data + size, buf_u, size /4);
            memcpy(YUVImage.data + size + size /4, buf_v, size / 4);
            cvtColor(YUVImage, BGRImage, CV_YUV2BGR_I420);
            // imshow("123",BGRImage);
            //   waitKey(10);
            return true;
        }else{
            prt(info,"decode a fail");
            return false;
        }
    }
    bool isOpened()
    {
        return opened;
    }
    void release()
    {
        release_input();
        release_decoder();
        opened=false;
    }
    int get(int cmd)
    {
        int ret;
        switch(cmd){
        case CV_CAP_PROP_FRAME_WIDTH:ret=width;break;
        case CV_CAP_PROP_FRAME_HEIGHT:ret=height;break;
        default:break;

        }
        return ret;
    }
private:
    bool init_input()
    {
        bool ret=true;
        av_register_all();
        avformat_network_init();
        fmt = avformat_alloc_context();
        if(avformat_open_input(&fmt, url.data(), NULL, 0) != 0) {
            ret=false;
        } else if (avformat_find_stream_info(fmt, NULL) < 0) {
            ret=false;
        }
        return ret;
    }
    bool init_decoder()
    {
        bool ret=false;
        //codec = avcodec_find_decoder(CODEC_ID_H264);
        codec = avcodec_find_decoder(AV_CODEC_ID_H264);
        codec_ctx = avcodec_alloc_context3(codec);
        if (avcodec_open2(codec_ctx, codec, NULL) >= 0){
            avframe=NULL;
            // avframe= avcodec_alloc_frame();av_frame_alloc();
            avframe= av_frame_alloc();
            if(avframe)
                ret=true;
        }
        else {
            prt(info,"fail to open decoder");
        }
        return ret;
    }
    void release_decoder()
    {
        av_free(avframe);
        avcodec_close(codec_ctx);
    }
    void release_input()
    {
        avformat_close_input(&fmt);
    }

    bool decode()
    {
        int got_picture = 0;
        int len = 0;

        while (av_pkt.size > 0) {
            // prt(info,"decoing size: %d",av_pkt.size);
            len = avcodec_decode_video2(codec_ctx, avframe, &got_picture, &av_pkt);
            if (len < 0) {
                return false;
            }
            if (got_picture)
            {
                buf_y = (unsigned char *) avframe->data[0];
                buf_u = (unsigned char *) avframe->data[1];
                buf_v = (unsigned char *) avframe->data[2];
                //      prt(info,"decode size %d  (%d  %d)",av_pkt.size,avframe->width,avframe->height);
                //                prt(info,"%d",av_pkt.pts);
                width=avframe->height;
                height=avframe->width;


                return true;

            }
            av_pkt.size -= len;
            av_pkt.data += len;
        }
        return false;
    }


public:
    AVCodec *codec;
    AVCodecContext *codec_ctx;
    AVFrame *avframe;
    AVFormatContext *fmt;
    AVPacket av_pkt;
    bool opened;
    string url;
    unsigned char *buf_y;
    unsigned char *buf_u;
    unsigned char *buf_v;
    int width;
    int height;
};
//#define USE_CVCAP 1

class VideoSourceTest
{
public:

    VideoSourceTest(string path):t1(bind(&VideoSourceTest::check_point,this)),frame_rate(0),vcap(path)
    {
        t1.start(1000);
        prt(info,"%s",path.data());
        url=path;
        quit_flg=false;
        //  thread(bind(&VideoSource::run,this)).detach();
        // _start_async(bind(&VideoSource::run,this));
        src_trd=new thread(bind(&VideoSourceTest::run,this));
    }
    ~VideoSourceTest()
    {
        prt(info,"quiting video: %s", url.data());
        quit_flg=true;
        if(src_trd->joinable())
            src_trd->join();
        prt(info,"quiting video: %s", url.data());
        delete src_trd;
        t1.stop();
        prt(info,"quiting video: %s", url.data());

        //  this_thread::sleep_for(chrono::seconds(1));
        prt(info,"quit video: %s", url.data());
    }
    inline string get_url()
    {
        return url;
    }
    bool get_size(int &w,int &h)
    {
        bool ret=false;
        if(vcap.isOpened()){
            ret=true;
            w=vcap.get(CV_CAP_PROP_FRAME_WIDTH);
            h=vcap.get(CV_CAP_PROP_FRAME_HEIGHT);
        }
        return ret;
    }
    bool get_frame(Mat &frame)
    {
        int ret=false;
        lock.lock();
        if(frame_list.size()>0){
            frame_list.front().copyTo(frame);
            frame_list.pop_front();
            ret=true;
        }else{
            ret=false;
        }
        lock.unlock();
        return ret;
    }

    bool get_frame(Mat &frame, int &timestamp)
    {
        int ret=false;
        lock.lock();

        if(frame_list.size()>0){
            frame_list.front().copyTo(frame);


            //            int start_time=get_ms();
            //  prt(info,"%d,%d",frame.cols,frame.rows);
            //           resize(frame,frame,Size( 640,480),CV_INTER_LINEAR);
            //           int end_time=get_ms();
            //           prt(info,"%d",end_time-start_time);


            frame_list.pop_front();
            timestamp=cur_ms_list.front();
            cur_ms_list.pop_front();
            ret=true;
        }else{
            ret=false;
        }
        lock.unlock();
        return ret;
    }
private:
    void run()
    {
    #ifdef USE_CVCAP
        vcap=VideoCapture(url);
    #else
        vcap=PdVideoCaptureSdk(url);
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
                    vcap=PdVideoCaptureSdk(url);
    #endif
                }
                if(frame.cols==0){
                    vcap.release();
                    prt(info,"%s get frame error,retrying ... ", url.data());
                    continue;
                }else{
                    // int ts=vcap.get(CV_CAP_PROP_POS_MSEC);
                    //     int ts=vcap.get(CV_CAP_PROP_POS_FRAMES);;
                    //    int ts=vcap.get(CV_CAP_PROP_FRAME_COUNT);

                    int ts=vcap.get(CV_CAP_PROP_POS_AVI_RATIO);
                    //prt(info,"timestamp  %dms", ts);
                    frame_rate++;
                    lock.lock();
                    if(!(frame_rate%30))
                    {
                        //prt(info,"running %s",url.data());
                    }
                    if(frame_list.size()<3&&frame.rows>0&&frame.cols>0){
                        frame_list.push_back(frame);
                        cur_ms_list.push_back(ts);
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
                vcap=PdVideoCaptureSdk(url);
    #endif
                prt(info,"open url err:%s",url.data());
            }
        }
        prt(info,"thread is quiting");
        if( vcap.isOpened())
            vcap.release();
    }


    void check_point()
    {
        if(vcap.isOpened()){
            double w= vcap.get(CV_CAP_PROP_POS_FRAMES);
        }
    }
private:
#ifdef USE_CVCAP
    VideoCapture  vcap;
#else
    PdVideoCaptureSdk vcap;
#endif
    list <Mat> frame_list;
    list <int> cur_ms_list;

    int frame_wait_time;
    mutex lock;
    int frame_rate;
    string url;
    volatile bool quit_flg;
    Timer1 t1;

    thread *src_trd;
};

class Player
{
public:
    Player();
    bool  get(Mat & pic)
    {

       return src.get_frame(pic);

    }
private:
    VideoSourceTest src;
};

#endif // PLAYER_H
