#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QUdpSocket>
#include "tool.h"
#include "misc.h"
#include "playerwidget.h"
#include "app_data.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void start_udp()
    {
        int port = 12349;
        udpSocket = new QUdpSocket(this);
        bool result = udpSocket->bind(port);
        if(!result) {
            printf("err") ;
        }else{
            printf("ok") ;
        }

        connect(udpSocket, SIGNAL(readyRead()),
                this, SLOT(dataReceived()));
    }
private slots:
    void search_server_result(QString ip);
    void dataReceived()
    {
        while(udpSocket->hasPendingDatagrams()) {
            QByteArray datagram;
            datagram.resize(udpSocket->pendingDatagramSize());

            int sz= udpSocket->readDatagram(datagram.data(), datagram.size());
           // prt(info,"get data %d",sz);
            QString str(datagram.data());
            JsonPacket pkt(str.toStdString());
            AppOutputData rst( pkt  );
            if(cfg.CameraData.size()>=rst.CameraIndex){
                int cam_index=rst.CameraIndex;
                CameraInputData camera_cfg=cfg.CameraData[cam_index-1];
                thread_lock.lock();
                //prt(info,"recving cam %d",cam_index);
                if(players.size()<cam_index)
                {
                    prt(info,"recving cam %d, our sz %d ",cam_index,players.size());
                    thread_lock.unlock();
                    continue;
                }
                PlayerWidget *w= players[cam_index-1];
                w->set_output_data(rst.CameraOutput);
                thread_lock.unlock();
            }else{
                prt(info,"server output index %d,out of range(1- %d), make sure you\
             loaded the server cfg & camera size >0 ",rst.CameraIndex,cfg.CameraData.size());
            }
            //            thread_lock.lock();
            //            //prt(info,"recving cam %d",cam_index);
            //            if(players.size()<cam_index)
            //            {
            //                prt(info,"recving cam %d, our sz %d ",cam_index,players.size());
            //                thread_lock.unlock();
            //                continue;
            //            }
            //            PlayerWidget *w= players[cam_index-1];
            //            w->set_overlay(rst.CameraOutput);
            //            thread_lock.unlock();

        }
    }
    RequestPkt get_request_pkt(int op,int index, JsonPacket pkt)
    {
        return RequestPkt(op,index,pkt);
    }

    void on_pushButton_search_clicked();

    void on_comboBox_search_activated(const QString &arg1);

    void on_pushButton_addcam_clicked();

    void on_pushButton_delcam_clicked();

    void on_pushButton_load_clicked();
    void camera_request(RequestPkt req,PlayerWidget *wgt)
    {
        int idx=std::find(players.begin(),players.end(),wgt)-players.begin();
        prt(info,"req frome camera %d",idx+1);
        clt.send(get_request_pkt(AppInputData::MODIFY_CAMERA,idx+1,req.data()).data().str());
        //  RequestPkt req1=get_request_pkt(AppInputData::MODIFY_CAMERA,i+1,req);

    }
    void request_get_config()
    {
        //clt.get_config();
        RequestPkt req= AppInputData::get_request(AppInputData::GET_CONFIG,0,JsonPacket());
        clt.send(QByteArray(req.data().str().data()));
    }
    void request_add_cam(int pos, CameraInputData data)
    {
        //clt.get_config();
        RequestPkt req= AppInputData::get_request(AppInputData::INSERT_CAMERA,pos,data.data());
        clt.send(QByteArray(req.data().str().data()));
    }
    void request_del_cam(int index)
    {
        //clt.get_config();
        RequestPkt req= AppInputData::get_request(AppInputData::DELETE_CAMERA,index,JsonPacket());
        clt.send(QByteArray(req.data().str().data()));
    }
    void server_msg(QString msg);
    void widget_append_camera(CameraInputData d);
    void widget_remove_camera(QWidget * d);
    void start_config()
    {
        thread_lock.lock();
        for(CameraInputData d:cfg.CameraData)
        {
            widget_append_camera(d);
        }
        prt(info,"start config: %s",cfg.data().str().data());
        thread_lock.unlock();
    }
    void stop_config()
    {
        thread_lock.lock();

        for(PlayerWidget *w:players){
            widget_remove_camera(w);
            w->hide();
            delete w;
        }
        players.clear();
        thread_lock.unlock();
    }

    void on_pushButton_play_clicked();

    void on_pushButton_stop_clicked();

    void on_pushButton_clear_buffer_clicked();

private:
    Ui::MainWindow *ui;
    QUdpSocket *udpSocket;
    ServerInfoSearcher searcher;
    Client clt;
    vector<PlayerWidget*> players;
    AppInputData cfg;
    QMutex thread_lock;
};

#endif // MAINWINDOW_H
