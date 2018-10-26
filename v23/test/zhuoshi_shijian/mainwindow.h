#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tcpclient.h"
#include "configmanager.h"
#include "jsonpacket.h"
//#include <arpa/inet.h>
#include<winsock.h>
 #pragma comment(lib, "wsock32.lib")
#define HEAD_LEN 16
#define CMD0 0x1ffff
#define CMD1 0x40005
#define CMD2 0x20007

#define SYNC_CMD 0x10100
#define ACK_CMD 0x20100
namespace Ui {
class MainWindow;
}

typedef  unsigned int uint32;
typedef struct {

   uint32	cmd_start;		/* 0x0，用于命令的开始 */
   uint32	packet_length;	/* 命令的总长度，包头+包体 */
   uint32	packet_type;	/* 命令的类型 */
   uint32	flag;			/* 参数标志，默认为0 */

}SINOITSNetHead;

typedef struct {

   uint32	alarm_type;					/* 报警数据类型 */
   uint32	video_channel_NO;			/* 视频通道编号，如果一个相机就是一路视频时，也表示相机编号 */
   uint32	device_NO;					/* 设备编号 */
   char	device_name[32];			/* 设备名称 */
   uint32	record_NO;					/* 记录编号，由0开始自动增长，不足32位，前端补0 */
   uint32	scene_NO;					/* 发生场景的编号，从1开始 */
   char	scene_name[64];				/* 发生场景的名称 */
   uint32	record_time_s;			/* 发生记录时间（秒）*/
   uint32	record_time_ms;			/* 发生记录的毫秒部分 */
   uint32	alarm_action;				/* 报警行为 */
   uint32	goal_amounts;				/* 目标数目 */
   uint32	alarm_goal_amounts;			/* 报警目标数目 */
   uint32	event_type;					/* 事件类型 */
   uint32	event_keep_time_s;			/* 事件继续时间, 单位秒 */
   uint32	alarm_goal_location[4];		/* 报警目标在图片中矩形位置。(左、上、右、下顺序存储) */
   uint32	vichecle_speed;				/* 速度 */
   uint32	overspeed_alarm_threshold;	/* 超速报警门限 */
   uint32	low_speed_alarm_threshold;	/* 低速报警门限 */
   uint32	vichecle_flow;				/* 流量 */
   uint32	flow_alarm_threshold;		/* 流量超限报警阈值 */
   uint32	drip_object_size[2];		/* 抛洒物尺寸 宽高 */
   uint32	traffic_jam_degree;			/* 拥堵程度 0-100，表示百分率 */
   uint32	queue_length_threshold;		/* 排队长度门限，单位厘米 */
   uint32	queue_length;				/* 排队长度 */
   uint32	smokescope;					/* 烟雾浓度，0-100，表示百分率 */
   uint32	same_alarm_interval;		/* 同类报警时间间隔 */
   uint32	all_alarm_interval;			/* 所有报警时间间隔 */
   uint32	image_data_length[4];		/* 全景图片数据长度 */
   char	*image_data_ptr[4];			/* 全景图片数据指针 */
   char	append[2048];				/* 附加信息段，自定义 */

}SINOITSNetEvent;
class DetectionDevice:public JsonData{
public:
    string ip;
    int port;
    DetectionDevice(JsonPacket pkt):JsonData(pkt)
    {
        decode();
    }

    void encode()
    {
        ENCODE_STRING_MEM(ip);
        ENCODE_INT_MEM(port);
    }
    void decode()
    {

        DECODE_STRING_MEM(ip);
        DECODE_INT_MEM(port);
    }
};
class AppInputData:public JsonData{
public:

    vector <DetectionDevice> Devices;

    //AppInputData(JsonPacket pkt1):JsonDataWithTitle(pkt1,"DeviceConfig")
    AppInputData(JsonPacket pkt1):JsonData(pkt1)
    {
        decode();
    }
    AppInputData()
    {

    }
    void decode()
    {
        try{
            DECODE_JSONDATA_ARRAY_MEM(Devices);
        }catch(exception e){
            PRT_DECODE_EXCEPTION
        }
    }
    void encode()
    {
        try{
            ENCODE_JSONDATA_ARRAY_MEM(Devices);
        }catch(exception e){
            PRT_DECODE_EXCEPTION
        }
    }
};

class App
{
public:
    //  App():clt("192.168.1.216",41010,
    App():heart_timer(bind(&App::send_heart,this)),clt("192.168.1.55",41010)
    {
        Timer2 t2;
        t2.AsyncWait(10000,bind(&App::login,this));
        sql_need_connect=true;
       // heart_timer.start(3000);
    }
    void parse_flow(char *flow_buf,int len)
    {
        prt(info,"parsing flow data , length %d",len);
        unsigned int  camera_id=0;
        memcpy(&camera_id,flow_buf+4,4);
        unsigned int  device_no=0;
        memcpy(&device_no,flow_buf+8,4);
      //  device_no=ntohl(device_no);
        unsigned char name[32];
        memcpy(name,flow_buf+12,32);
        unsigned   int  start_ms=0;
        memcpy(&start_ms,flow_buf+44,4);
        prt(info,"%u, %u, %u, ",
            camera_id,device_no,start_ms);

        unsigned  int  period=0;
        memcpy(&period,flow_buf+48,4);

        unsigned  short lane_id[10];
        memcpy(&lane_id,flow_buf+52+20*0,20);

        unsigned   short big_car[10];
        memcpy(&big_car,flow_buf+52+20*1,20);

        unsigned  short small_car[10];
        memcpy(&small_car,flow_buf+52+20*2,20);

        unsigned  short average_speed[10];
        memcpy(&average_speed,flow_buf+52+20*3,20);

        unsigned  short max_queue[10];
        memcpy(&max_queue,flow_buf+52+20*4,20);

        unsigned  short dur_ocuppy[10];
        memcpy(&dur_ocuppy,flow_buf+52+20*5,20);

        unsigned  short dis_ocuppy[10];
        memcpy(&dis_ocuppy,flow_buf+52+20*6,20);
        unsigned  short average_dis[10];
        memcpy(&average_dis,flow_buf+52+20*7,20);
    }
    void parse_event(char *event_buf,int len)
    {
        prt(info,"parsing event data , length %d",len);
        unsigned int  camera_id=0;
        memcpy(&camera_id,event_buf+4,4);
        unsigned int  device_no=0;
        memcpy(&device_no,event_buf+8,4);
      //  device_no=ntohl(device_no);
        unsigned char name[32];
    }
    void handle_msg(char * da,int len)
    {
        //   prt(info,"get tcp size %d",len);

        unsigned int arg1=0;
        unsigned int arg2=0;
        unsigned int arg3=0;
        unsigned int arg4=0;
        //   printf("\nflush 258 \n");
        fflush(NULL);
        if(len>=HEAD_LEN){

            //prt(info,"try decode head--------------------");
            decode_head(da,arg1,arg2,arg3,arg4);
            // prt(info,"data size %u",arg2);
            // prt(info,"size %u ,cmd %x",arg2,arg3);
            if(arg3==ACK_CMD){
                //   prt(info,"======================> confirm login %u ",arg4);
                // parse_flow(da+HEAD_LEN,arg2-HEAD_LEN);
            }
            if(arg3==CMD0){
                fflush(NULL);
                prt(info,"======================> heart ");
                // parse_flow(da+HEAD_LEN,arg2-HEAD_LEN);
            }
            if(arg3==CMD1){
                prt(info,"======================> flow data");
                parse_flow(da+HEAD_LEN,arg2-HEAD_LEN);
            }
            if(arg3==CMD2){
                prt(info,"======================> event data");
              //  parse_event(da+HEAD_LEN,arg2-HEAD_LEN);

            }
            //  prt(info,"flg %d",arg4);
            // prt(info,"  decode done--------------------");
        }

    }

//    DatabaseInstance  &  get_database()
//    {
//#if 1
//        DatabaseInstance &ins=DatabaseInstance::get_instance();
//        string user="root";
//        string passwd="myadmin";
//        string db="datainfo";
//        string host="192.168.1.4";
//        if(sql_need_connect){
//            ins.connect(host,user,passwd,db);
//            sql_need_connect=false;
//        }
//#else

//        DatabaseInstance &ins=DatabaseInstance::get_instance();
//        //        if(sql_need_connect){
//        //                ins.disconnect();sql_need_connect=false;
//        //        }
//        string user="root";
//        string passwd="root";
//        string db="AIPD";
//        string host="192.168.1.219";
//        if(sql_need_connect){
//            ins.connect(host,user,passwd,db);
//            sql_need_connect=false;
//        }
//#endif

//        return ins;
//    }
    void send_heart()
    {

        //   DatabaseInstance &ins=get_database();

        // ins.query("select * from  event;");
        // ins.query("select * from  car_tg;");
        //   ins.print_table();


        prt(info," send heart ");
        //          char test[]="123";
        //          clt.send((char*)test,3);
        char buf_head[HEAD_LEN];

        encode_head(buf_head,0,HEAD_LEN,SYNC_CMD,0);
        clt.send((char*)buf_head,HEAD_LEN);
    }
    inline void encode_head(char *buf,const unsigned int arg1,const unsigned int arg2,
                            const unsigned int arg3,const unsigned int arg4)
    {
        memset(buf,0,HEAD_LEN);
        int tmp1=htonl(arg2);
        int tmp2=htonl(arg3);
        //  *(buf1+8)=tmp;//wrong !!!!
        memcpy(buf+4,&tmp1,4);
        memcpy(buf+8,&tmp2,4);
    }
    inline void decode_head(char *buf,unsigned int &arg1,unsigned int &arg2,
                            unsigned int &arg3,unsigned int &arg4)
    {
        int sz=0;
        memcpy(&arg1,buf+4*0,4);
        memcpy(&arg2,buf+4*1,4);
        memcpy(&arg3,buf+4*2,4);
        memcpy(&arg4,buf+4*3,4);

        //        arg1=ntohl(*(buf+4*0));
        //        arg2=ntohl(*(buf+4*1));
        ////        memcpy(&sz,buf+4,4);
        ////         arg2=(sz);
        //        arg3=ntohl(*(buf+4*2));
        //        arg4=ntohl(*(buf+4*3));
    }
    void login()
    {
#if 0
        prt(info,"try to login");
        unsigned char buf1[80];
        memset(buf1,0,80);
        int tmp1=htonl(80);
        int tmp2=htonl(0x00010100);
        //  *(buf1+8)=tmp;//wrong !!!!
        memcpy(buf1+4,&tmp1,4);
        memcpy(buf1+8,&tmp2,4);
        unsigned char name[]="admin";
        unsigned char passwd[]="admin";
        strcpy((char*)buf1+16,(char*)name);
        strcpy((char*)buf1+48,(char*)passwd);
        clt.send((char*)buf1,80);
#else
        char buf_login[80];
        encode_head(buf_login,0,16+64,0x00010100,0);
        unsigned char name[]="admin";
        unsigned char passwd[]="admin";
        strcpy((char*)buf_login+HEAD_LEN,(char*)name);
        strcpy((char*)buf_login+HEAD_LEN+32,(char*)passwd);
        clt.send((char*)buf_login,80);
#endif
    }
private:
    TcpClient clt;
    Timer1 heart_timer;
    bool sql_need_connect;
};
class Client:public QObject
{
    Q_OBJECT
public:
    Client(string ip,int port):clt(ip,port)
    {
        connect(&clt,SIGNAL(server_msg(char*,int)),SLOT(handle_msg(char*,int)));
        connect(&tmr,SIGNAL(timeout()),SLOT(send_heart()));
        tmr.start(1000);
    }
    void login()
    {
        char buf_login[80];
        encode_head(buf_login,0,16+64,0x00010100,0);
        unsigned char name[]="admin";
        unsigned char passwd[]="admin";
        strcpy((char*)buf_login+HEAD_LEN,(char*)name);
        strcpy((char*)buf_login+HEAD_LEN+32,(char*)passwd);
        clt.send((char*)buf_login,80);
    }
    inline void encode_head(char *buf,const unsigned int arg1,const unsigned int arg2,
                            const unsigned int arg3,const unsigned int arg4)
    {
        memset(buf,0,HEAD_LEN);
        int tmp1=htonl(arg2);
        int tmp2=htonl(arg3);
        //  *(buf1+8)=tmp;//wrong !!!!
        memcpy(buf+4,&tmp1,4);
        memcpy(buf+8,&tmp2,4);
    }
    inline void decode_head(char *buf,unsigned int &arg1,unsigned int &arg2,
                            unsigned int &arg3,unsigned int &arg4)
    {
        int sz=0;
        memcpy(&arg1,buf+4*0,4);
        memcpy(&arg2,buf+4*1,4);
        memcpy(&arg3,buf+4*2,4);
        memcpy(&arg4,buf+4*3,4);

        //        arg1=ntohl(*(buf+4*0));
        //        arg2=ntohl(*(buf+4*1));
        ////        memcpy(&sz,buf+4,4);
        ////         arg2=(sz);
        //        arg3=ntohl(*(buf+4*2));
        //        arg4=ntohl(*(buf+4*3));
    }
public slots:
    void send_heart()
    {
      //  prt(info," send heart ");
        //          char test[]="123";
        //          clt.send((char*)test,3);
        char buf_head[HEAD_LEN];

        encode_head(buf_head,0,HEAD_LEN,SYNC_CMD,0);
        clt.send((char*)buf_head,HEAD_LEN);
    }
    void handle_msg(char *buffer, int len)
    {
        prt(info," recv msg len %d",len);

        //   prt(info,"get tcp size %d",len);

        unsigned int arg1=0;
        unsigned int arg2=0;
        unsigned int arg3=0;
        unsigned int arg4=0;

        if(len>=HEAD_LEN){

            //prt(info,"try decode head--------------------");
            decode_head(buffer,arg1,arg2,arg3,arg4);
            // prt(info,"data size %u",arg2);
            // prt(info,"size %u ,cmd %x",arg2,arg3);
            if(arg3==ACK_CMD){
                prt(info,"======================> confirm login rst %u ",arg4);
                // parse_flow(da+HEAD_LEN,arg2-HEAD_LEN);
            }
            if(arg3==CMD0){
                fflush(NULL);
                prt(info,"======================> heart ");
                // parse_flow(da+HEAD_LEN,arg2-HEAD_LEN);
            }
            if(arg3==CMD1){
                prt(info,"======================> flow data");
                parse_flow(buffer+HEAD_LEN,arg2-HEAD_LEN);
            }
            if(arg3==CMD2){
                prt(info,"======================> event data");
            //    parse_event(buffer+HEAD_LEN,arg2-HEAD_LEN);
                    unpacketTcp(buffer);
            }
            //  prt(info,"flg %d",arg4);
            // prt(info,"  decode done--------------------");
        }
    }
    void parse_flow(char *flow_buf,int len)
    {
        prt(info,"parsing flow data , length %d",len);
        unsigned int  camera_id=0;
        memcpy(&camera_id,flow_buf+4,4);
        unsigned int  device_no=0;
        memcpy(&device_no,flow_buf+8,4);
      //  device_no=ntohl(device_no);
        unsigned char name[32];
        memcpy(name,flow_buf+12,32);
        unsigned   int  start_ms=0;
        memcpy(&start_ms,flow_buf+44,4);
        prt(info,"%u, %u, %u, ",
            camera_id,device_no,start_ms);

        unsigned  int  period=0;
        memcpy(&period,flow_buf+48,4);

        unsigned  short lane_id[10];
        memcpy(&lane_id,flow_buf+52+20*0,20);

        unsigned   short big_car[10];
        memcpy(&big_car,flow_buf+52+20*1,20);

        unsigned  short small_car[10];
        memcpy(&small_car,flow_buf+52+20*2,20);

        unsigned  short average_speed[10];
        memcpy(&average_speed,flow_buf+52+20*3,20);

        unsigned  short max_queue[10];
        memcpy(&max_queue,flow_buf+52+20*4,20);

        unsigned  short dur_ocuppy[10];
        memcpy(&dur_ocuppy,flow_buf+52+20*5,20);

        unsigned  short dis_ocuppy[10];
        memcpy(&dis_ocuppy,flow_buf+52+20*6,20);
        unsigned  short average_dis[10];
        memcpy(&average_dis,flow_buf+52+20*7,20);
    }

    void unpacketTcp(char *data)
    {
        SINOITSNetEvent packetEvent;

        if(data == NULL) {
            return;
        }
        char recodeType[32] = "";
      //  packetHead.flag = (*(uint32 *)&data[12]);
//        if(CM == packetHead.flag) {
//            memcpy(recodeType, "实时记录", sizeof("实时记录"));
//        } else if (NON_REALTIME_RECORD == packetHead.flag) {
//            memcpy(recodeType, "非实时记录", sizeof("非实时记录"));
//        }
    //   printf("记录类型：%s\n", recodeType);

  //      packetHead.packet_length = (*(uint32 *)&data[4]);
   //    prt(info, "包总长度：%d", packetHead.packet_length);
 packetEvent.alarm_type = *(uint32 *)&data[16];
        packetEvent.video_channel_NO = *(uint32 *)&data[20];
        printf("通道号: %d\n", packetEvent.video_channel_NO);

        packetEvent.device_NO = (*(uint32 *)&data[24]);
        printf("device_NO: %d\n", packetEvent.device_NO);

        memcpy(packetEvent.device_name, data + 28, 32);
        printf("device_name: %s\n", packetEvent.device_name);

        memcpy(packetEvent.scene_name, data + 68, 64);

        packetEvent.record_time_s = (*(uint32 *)&data[132]);
      //  packetEvent.record_time_ms = (*(uint32 *)&data[136]);

        packetEvent.event_type = (*(uint32 *)&data[152]);
        printf("event type 0x%08x\n", packetEvent.event_type);

        memcpy(packetEvent.image_data_length, data + 228, 16);

        prt(info,"设备编号：%d", packetEvent.device_NO);
         prt(info,"设备名称：%s", packetEvent.device_name);
         prt(info, "事件类型：0x%08x", packetEvent.device_NO);
        prt(info, "场景名称：0x%08x", packetEvent.scene_name);

        prt(info, "数据长度：%d, %d, %d, %d", packetEvent.image_data_length[0], packetEvent.image_data_length[1], packetEvent.image_data_length[2], packetEvent.image_data_length[3]);


        int temlen = 0;
        char *path[4];
        for(int i = 0; i < 4; i++)
        {
            if(packetEvent.image_data_length[i] > 0)
            {
                packetEvent.image_data_ptr[i] = (char*)malloc(packetEvent.image_data_length[i]);
                memcpy(packetEvent.image_data_ptr[i], data + 244 + temlen, packetEvent.image_data_length[i]);
                temlen += packetEvent.image_data_length[i];
                path[i] = (char* )malloc(128);
                printf("path: %s\n", packetEvent.image_data_ptr[i]);
                if(i == 0)   prt(info, "图片数据：%s", packetEvent.image_data_ptr[0]);
                if(i == 1)   prt(info,"视频数据：%s", packetEvent.image_data_ptr[1]);
                //myStrtok(packetEvent.image_data_ptr[i], "\\", &path[i], dir);
                //doneVideoOrImg(path);
            }
        }


        prt(info, "设备编号：%d", packetEvent.device_NO);
        prt(info, "设备名称：%s", packetEvent.device_name);
         prt(info, "事件类型：0x%08x", packetEvent.device_NO);
        prt(info, "场景名称：0x%08x", packetEvent.scene_name);

         prt(info,"数据长度：%d, %d, %d, %d", packetEvent.image_data_length[0], packetEvent.image_data_length[1], packetEvent.image_data_length[2], packetEvent.image_data_length[3]);
        //log(__FILE__, __LINE__, __FUNCTION__, "图片数据：%s", packetEvent.image_data_ptr[0]);
        //log(__FILE__, __LINE__, __FUNCTION__, "视频数据：%s", packetEvent.image_data_ptr[1]);
        printf("设备编号：%d\n", packetEvent.device_NO);
        printf("设备名称：%s\n", packetEvent.device_name);
        printf("事件类型：0x%08x\n", packetEvent.device_NO);

        printf("图片长度：%d, %d, %d, %d\n", packetEvent.image_data_length[0], packetEvent.image_data_length[1], packetEvent.image_data_length[2], packetEvent.image_data_length[3]);
        printf("图片数据：%s\n", packetEvent.image_data_ptr[0]);
        printf("视频数据：%s\n", packetEvent.image_data_ptr[1]);
        free(packetEvent.image_data_ptr[0]);
        free(packetEvent.image_data_ptr[1]);
     //   char *type = "";
      //  chooseEventType(packetEvent.event_type, &type);
      //  printf("event type: %s\n", type);

//        struct tm *p;
//        time_t tp = packetEvent.record_time_s;
//        //time(&tp);
//        p = localtime(&tp);
//        printf("%d-%02d-%02d %02d:%02d:%02d\n", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);

//        char aTime[64];
//        sprintf(aTime, "%d-%02d-%02d %02d:%02d:%02d", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
//        prt(info, "事件时间：%s", aTime);

//        char xml[128] = "";
//        sprintf(xml, "D:\\事件警报\\data\\%04d_%02d_%02d.xml", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday);

     //   addDataToXml(xml, packetEvent, type, aTime, path[1], path[0]);

//        if(iToday == dateDay)
//        {
//            if(bNextDay)
//            {
//                m_listCtrl.DeleteAllItems();
//                nItem = 0;
//                bNextDay = false;
//            }
//            CString devName(packetEvent.device_name);
//            CString devNO, channelNO;
//            CString eType(type);
//            CString eventTime(aTime);
//            CString num;
//            num.Format(_T("%d"), nItem + 1);
//            //devName.Format(_T("%s"), packetEvent.device_name);
//            devNO.Format(_T("%d"), packetEvent.device_NO);
//            channelNO.Format(_T("%d"), packetEvent.video_channel_NO);
//            m_listCtrl.InsertItem(nItem, num);
//            m_listCtrl.SetItemText(nItem, 1, devName);
//            m_listCtrl.SetItemText(nItem, 2, eType);
//            m_listCtrl.SetItemText(nItem, 3, eventTime);
//            m_listCtrl.SetItemText(nItem, 4, devNO);
//            m_listCtrl.SetItemText(nItem, 5, channelNO);

//            /*if(packetEvent.image_data_length[0] == 0) {
//                log(__FILE__, __LINE__, __FUNCTION__, "无图片数据");
//            } else {
//                memcpy(packetEvent.append, data + 248, packetEvent.image_data_length[0]);
//                saveImg((char*)packetEvent.append, packetEvent.image_data_length[0], packetEvent.record_time_s);
//            }*/
//            nItem++;
//        }
    }

    void parse_event(char *event_buf,int len)
    {
        prt(info,"parsing event data , length %d",len);
        //
        unsigned int  alarm_type=0;
        memcpy(&alarm_type,event_buf+4*0,4);

           }
private:
    TcpClient clt;
    QTimer tmr;
};

//class DetectionDevice:public JsonData{
//public:
//    string ip;
//    int port;
//    DetectionDevice(JsonPacket pkt):JsonData(pkt)
//    {

//    }

//    void encode()
//    {
//        ENCODE_STRING_MEM(ip);
//        ENCODE_INT_MEM(port);
//    }
//    void decode()
//    {
//        DECODE_STRING_MEM(ip);
//        DECODE_INT_MEM(port);
//    }
//};
//class AppInputData:public JsonData{
//public:

//    vector <DetectionDevice> Devices;

//    //AppInputData(JsonPacket pkt1):JsonDataWithTitle(pkt1,"DeviceConfig")
//    AppInputData(JsonPacket pkt1):JsonData(pkt1)
//    {
//        decode();
//    }
//    AppInputData()
//    {

//    }
//    void decode()
//    {
//        try{
//            DECODE_JSONDATA_ARRAY_MEM(Devices);
//        }catch(exception e){
//            PRT_DECODE_EXCEPTION
//        }
//    }
//    void encode()
//    {
//        try{
//            ENCODE_JSONDATA_ARRAY_MEM(Devices);
//        }catch(exception e){
//            PRT_DECODE_EXCEPTION
//        }
//    }
//};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
  // TcpClient clt;
  //  App app;
    ConfigManager cm;
    vector<Client *> clts;
    //ConfigManager cm;
};

#endif // MAINWINDOW_H
