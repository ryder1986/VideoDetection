#ifndef DahuaVideoCapture_H
#define DahuaVideoCapture_H
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
#include "opencv2/opencv.hpp"
#include "tool.h"
#include "dhnetsdk.h"
#include "dhconfigsdk.h"
#include "UserDefine.h"
#include <mutex>
using namespace std;
using namespace cv;
static   int loop;
static  int old_time;
static  int new_time;
vector<AVPacket > g_pkts;
mutex pkts_lock;
class DahuaVideoCapture{
public:
    DahuaVideoCapture(string url):url(url)
    {
        opened=false;
        width=0;
        height=0;
        if(!init_input()){

        }

        if(!init_decoder()){

        }

    }
    ~DahuaVideoCapture()
    {

    }
    bool read(Mat &BGRImage)
    {
        //  return false;
        // int t= cvNamedWindow("123");
        Mat YUVImage;
        int size;
        size=width*height;
        av_init_packet(&av_pkt);
#if 0
        if ((av_read_frame(fmt, &av_pkt) < 0)) {
            prt(info,"read frm fail");
            return false;
        }
#else
        pkts_lock.lock();
        if(g_pkts.size()>0){
            AVPacket pkt_tmp=g_pkts.front();
            g_pkts.erase(g_pkts.begin());
            int sz=pkt_tmp.size;
            av_pkt.data=pkt_tmp.data;
            av_pkt.size=sz;
        }else{
            pkts_lock.unlock();
            return false;
        }
        pkts_lock.unlock();
#endif
        int64_t dt,wt;
        //av_get_output_timestamp(fmt,0,&dt,&wt);
        if(decode()){

            YUVImage.create(width*3/2, height, CV_8UC1);
            memcpy(YUVImage.data, buf_y, size);
            memcpy(YUVImage.data + size, buf_u, size /4);
            memcpy(YUVImage.data + size + size /4, buf_v, size / 4);
            cvtColor(YUVImage, BGRImage, CV_YUV2BGR_I420);
            free(av_pkt.data);
            av_free_packet(&av_pkt);
            // imshow("123",BGRImage);
            //   waitKey(10);
            return true;
        }else{
            free(av_pkt.data);
            av_free_packet(&av_pkt);
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
    static void CALLBACK DisConnectFunc1(LLONG lLoginID, char *pchDVRIP, LONG nDVRPort, LDWORD dwUser)
    {

        return;
    }
    static void CALLBACK RealDataCallBack(LLONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, LDWORD dwUser)
    {
        loop++;
        new_time=get_time_second();
        if(new_time-old_time>=1){

            prt(info,"fps %d , type  %d, size  %d",loop,dwDataType,dwBufSize);

            loop=0;
            old_time=new_time;
        }
        DH_Channel_Info* pChannelInfo = (DH_Channel_Info*)dwUser;
        if(NULL == pChannelInfo)
        {

            return;
        }else{

        }
        // pkts_lock.lock();
        if(dwBufSize>1000){
            AVPacket pkt_tmp;
            av_init_packet(&pkt_tmp);
            av_new_packet(&pkt_tmp,dwBufSize);
            int sz=pkt_tmp.size=dwBufSize;
            memcpy( pkt_tmp.data,pBuffer,sz);
            pkt_tmp.size=sz;
            g_pkts.push_back(pkt_tmp);
        }
        // pkts_lock.unlock();
        pChannelInfo->dwStatistic = dwBufSize;
    }
    bool init_input()
    {
        bool ret=true;
        av_register_all();
        CLIENT_Init(DisConnectFunc1, (LDWORD)NULL);
        return ret;
    }
    bool init_decoder()
    {

        bool ret=false;
        int nRetLen = 0;
        m_lLoginHandle = CLIENT_LoginEx2("192.168.1.108", 37777, "admin","admin12345",EM_LOGIN_SPEC_CAP_TCP, NULL, &deviceInfo, &error);
        int count;
        NET_DEV_CHN_COUNT_INFO stuChn = {sizeof(NET_DEV_CHN_COUNT_INFO)};
        if(CLIENT_QueryDevState(m_lLoginHandle, DH_DEVSTATE_DEV_CHN_COUNT, (char*)&stuChn, stuChn.dwSize, &nRetLen))
        {
            count = stuChn.stuVideoIn.nMaxTotal;
        }
        else
        {
            count= deviceInfo.nChanNum;
        }
        prt(info,"count %d",count);
        LLONG lRealPlayHandle = CLIENT_RealPlayEx(m_lLoginHandle,0, NULL,DH_RType_Realplay_1);
        CLIENT_SetRealDataCallBack(lRealPlayHandle, RealDataCallBack, (LDWORD)&info);

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
    AVPacket av_pkt;
    bool opened;
    string url;
    unsigned char *buf_y;
    unsigned char *buf_u;
    unsigned char *buf_v;
    int width;
    int height;
    vector<AVPacket > pkts;
    LLONG m_lLoginHandle;
    NET_DEVICEINFO_Ex deviceInfo = {0};
    DH_Channel_Info info;
    int error;

};

#endif // DahuaVideoCapture_H
