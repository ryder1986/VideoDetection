#ifndef APP_H
#define APP_H
#include "server.h"
#define HEAD_LEN 16
#define CMD1 0x40005
#define CMD2 0x20007

class App
{
public:
    App():clt("192.168.1.55",41010,
              bind(&App::handle_msg,this,placeholders::_1,placeholders::_2))
      ,heart_timer(bind(&App::send_heart,this))
    {
        Timer2 t2;
        t2.AsyncWait(1000,bind(&App::login,this));

        heart_timer.start(3000);
    }
    void handle_msg(char * da,int len)
    {
        prt(info,"get tcp size %d",len);

        unsigned int arg1=0;
        unsigned int arg2=0;
        unsigned int arg3=0;
        unsigned int arg4=0;

        if(len>=HEAD_LEN){
            prt(info,"try decode head--------------------");
            decode_head(da,arg1,arg2,arg3,arg4);
            prt(info,"data size %u",arg2);
            prt(info,"cmd %x",arg3);
            if(arg3==CMD1){
                 prt(info,"======================> flow data");
            }
            if(arg3==CMD2){
                 prt(info,"======================> event data");
            }
            prt(info,"flg %d",arg4);
            prt(info,"  decode done--------------------");
        }

    }
    void send_heart()
    {
        prt(info," send heart ");
        char buf_head[HEAD_LEN];
        encode_head(buf_head,0,HEAD_LEN,0x00010100,0);
        clt.send((char*)buf_head,HEAD_LEN);
    }
    inline void encode_head(char *buf,const unsigned int arg1,const unsigned int arg2,
                            const unsigned int arg3,const unsigned int arg4)
    {
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
};

#endif // APP_H
