#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QtNetwork>
#include <QTcpServer>
#include <QTcpSocket>
#include "tool.h"
class TcpClient:public QObject
{
    Q_OBJECT
public:
    TcpClient(string ip, int pt)
    {
        ipaddr=ip;
        port=pt;
   //     skt.connectToHost("192.168.1.216",port);
        skt.connectToHost(ipaddr.data(),port);
        connect(&skt,SIGNAL(readyRead()),this,SLOT(handle_msg()));
        //     t1.start(1000);

        connect(&watch_dog,SIGNAL(timeout()),this,SLOT(check_connection()));

        watch_dog.start(1000);
    }
    string get_ip()
    {
        return ipaddr;
    }

    void send_msg()
    {
        skt.write("123",3);
    }
    void send(char *buf,int len)
    {
        int ret=skt.write(buf,len);
        prt(info,"send  %d bytes",ret);
    }
public slots:
    void handle_msg()
    {
        QByteArray msg=skt.readAll();
       // prt(info,"sz %d",msg.size());
      //  recv_fun(msg.data(),msg.size());
        emit  server_msg(msg.data(),msg.size());
    }
    void check_connection()
    {
        prt(info,"connect state  %d",skt.state());

        //         if(skt.state()==QTcpSocket::ConnectingState)
        //         {
        //             skt.disconnectFromHost();
        //         }
        if(skt.state()==QTcpSocket::UnconnectedState)
            skt.connectToHost(ipaddr.data(),port);
    }
signals:
    void server_msg(char *msg,int len);
private:
    QTcpSocket skt;
 //   Timer1 t1;
    QTimer watch_dog;
    string ipaddr;
    int port;
    //function <void(char *data,int len)> recv_fun=[](char *data,int len){ cout<<"process recv data"<<endl;};
};

#endif // TCPCLIENT_H
