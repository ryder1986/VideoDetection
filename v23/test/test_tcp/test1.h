#ifndef TEST1_H
#define TEST1_H

#include "tool.h"
#include  "server.h"
class Test1
{

    TcpClient client;
public:
    Test1();
    void handle_msg(char * da,int len)
    {
        prt(info,"get ->> %s",da);
    }
};

class Test2
{

    TcpClient client;
    // Timer1 t1;
public:
    Test2(): client("192.168.1.55",41010,bind(&Test2::handle_msg,this,placeholders::_1,placeholders::_2))
      //    Test2(): client("192.168.1.219",6789,bind(&Test2::handle_msg,this,placeholders::_1,placeholders::_2))
      //  , t1(bind(&Test2::test_send,this))

    {
        Timer2 t2;
        t2.AsyncWait(1000,bind(&Test2::send_passwd,this));


        //t1.start(1000);
    }

    void handle_msg(char * da,int len)
    {
        prt(info,"get size %d",len);
    }

    void test_send()
    {
        prt(info,"send");
        char buf[]="aaaa";
        client.send(buf,3);
        prt(info,"send done");
    }
    inline  int h2n(int s)
    {
        int tmp=s;
        char *t1=(char *)&tmp;
        char *t2=(char *)&s;
        t2[0]=t1[3];
        t2[1]=t1[2];
        t2[2]=t1[1];
        t2[3]=t1[0];
        return s;
    }

    void send_passwd()
    {
        prt(info,"send");
        unsigned char buf1[80];
        memset(buf1,0,80);
        //  *buf1=(int)(0x0);
        // *(buf1+4)=htonl(80);

        int tmp1=htonl(80);
        int tmp2=htonl(0x00010100);
             //  *(buf1+8)=tmp;//wrong !!!!
        memcpy(buf1+4,&tmp1,4);
        memcpy(buf1+8,&tmp2,4);
        unsigned char name[]="admin";
        unsigned char passwd[]="admin";
        strcpy((char*)buf1+16,(char*)name);
        strcpy((char*)buf1+48,(char*)passwd);
        client.send((char*)buf1,80);
        prt(info,"send done");
    }

};

#endif // TEST1_H
