#ifndef APP_H
#define APP_H
#include "server.h"
#include <mysql/mysql.h>
#define HEAD_LEN 16
#define CMD0 0x1ffff
#define CMD1 0x40005
#define CMD2 0x20007

#define SYNC_CMD 0x10100
#define ACK_CMD 0x20100

#include "jsonpacket.h"
class DetectionDevice:public JsonData{
public:
    string ip;
    int port;
    DetectionDevice(JsonPacket pkt):JsonData(pkt)
    {

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

class DatabaseInstance
{
public:
    static DatabaseInstance &get_instance()
    {
        static DatabaseInstance ins;
        return ins;
    }
    bool connect()
    {
        if(!mysql_real_connect(sql,host.data(),user.data(),passwd.data(),NULL,3306,0,0)){
            sql=NULL;
            const char *err= mysql_error(sql);
            prt(info,"connect database error: %s" ,err);
            mysql_close(sql);
            return false;
        }else{
            prt(info,"connect database %s ok",db.data());
            int ret=  mysql_select_db(sql,db.data());
            prt(info,"ok use %d",ret);
            return true;
        }
    }

    bool connect(string host,string user,string passwd,string db)
    {
        if(!mysql_real_connect(sql,host.data(),user.data(),passwd.data(),NULL,3306,0,0)){
            sql=NULL;
            const char *err= mysql_error(sql);
            prt(info,"connect database error: %s" ,err);
            mysql_close(sql);
            return false;
        }else{
            prt(info,"connect database %s ok",db.data());
            int ret=  mysql_select_db(sql,db.data());
            prt(info,"ok use %d",ret);
            return true;
        }
    }
    void print_result()
    {
    }
    void print_table()
    {
        if(!query_flag){
            prt(info,"eror:can not print table  because query failed");
            return ;
        }
#if 1
        //   MYSQL_FIELD *field;
        if(!resid)
            return;
        while((field = mysql_fetch_field(resid)))
        {
            printf(" %s ", field->name);
        }
#endif

        cout<<endl;
        // query("select * from  table1");
        //  resid = mysql_store_result(sql);
        MYSQL_ROW row;
        unsigned int num_fields;
        unsigned int i;

        num_fields = mysql_num_fields(resid);
        // return ;
        while ((row = mysql_fetch_row(resid)))
        {
            unsigned long *lengths;
            lengths = mysql_fetch_lengths(resid);
            for(i = 0; i < num_fields; i++)
            {
                //  field = mysql_fetch_field(resid);
                //  printf("field   %p\n", field);
                // printf("field name %s\n", field->name);
                printf("[%.*s] ", (int) lengths[i],
                       row[i] ? row[i] : "NULL");
            }
            printf("\n");
        }
    }
    void query(char *statement)
    {

        //mysql_free_result(resid );
        int mysqlret = mysql_real_query(sql,statement,(unsigned int)strlen(statement));
        if(mysqlret!=0)
        {
            query_flag=false;
            //       abort();
            //  assert(false);
            const char *err_str=  mysql_error(sql);
            prt(info,"error: %s" , err_str);
        }else{
            query_flag=true;
            resid = mysql_store_result(sql);
        }
    }
    ~DatabaseInstance()
    {
        mysql_close(sql);
    }
    void disconnect()
    {
        mysql_close(sql);
    }

private:
    DatabaseInstance(char *host,char *user,char*passwd,char *db):
        host(host),user(user),passwd(passwd),db(db),sql(NULL),query_flag(false),need_connect(true)
    {
        sql=mysql_init(NULL);
    }

    DatabaseInstance()
    {
        sql=mysql_init(NULL);
    }
    string host;
    string user;
    string passwd;
    string db;
    MYSQL *sql;
    MYSQL_RES *resid;
    MYSQL_FIELD *field;
    bool query_flag;
    bool need_connect;
    // MYSQL_ROW row_str_list;
};

class App
{
public:
    //  App():clt("192.168.1.216",41010,
    App():clt("192.168.1.55",41010,
              bind(&App::handle_msg,this,placeholders::_1,placeholders::_2))
      ,heart_timer(bind(&App::send_heart,this))
    {
        Timer2 t2;
        t2.AsyncWait(1000,bind(&App::login,this));
        sql_need_connect=true;
        heart_timer.start(3000);
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
        memcpy(&camera_id,flow_buf+4,4);
        unsigned int  device_no=0;
        memcpy(&device_no,flow_buf+8,4);
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
                parse_event(da+HEAD_LEN,arg2-HEAD_LEN);
            }
            //  prt(info,"flg %d",arg4);
            // prt(info,"  decode done--------------------");
        }

    }

    DatabaseInstance  &  get_database()
    {
#if 1
        DatabaseInstance &ins=DatabaseInstance::get_instance();
        string user="root";
        string passwd="myadmin";
        string db="datainfo";
        string host="192.168.1.4";
        if(sql_need_connect){
            ins.connect(host,user,passwd,db);
            sql_need_connect=false;
        }
#else

        DatabaseInstance &ins=DatabaseInstance::get_instance();
        //        if(sql_need_connect){
        //                ins.disconnect();sql_need_connect=false;
        //        }
        string user="root";
        string passwd="root";
        string db="AIPD";
        string host="192.168.1.219";
        if(sql_need_connect){
            ins.connect(host,user,passwd,db);
            sql_need_connect=false;
        }
#endif

        return ins;
    }
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

#endif // APP_H
