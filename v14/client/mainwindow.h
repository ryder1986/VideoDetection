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
    enum MODE{
        ONE_CAM=1,
        ALL_CAM
    };

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
    void play_one_cam(int index)
    {
        play_index=index;
        play_mode=ONE_CAM;
        stop_config();
        start_a_cam(index);
    }
    void play_all_cam()
    {
        stop_config();
        play_mode=ALL_CAM;
        start_config();
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
                if(play_mode==ALL_CAM){
                    int cam_index=rst.CameraIndex;
                    CameraInputData camera_cfg=cfg.CameraData[cam_index-1];
                    thread_lock.lock();
                    //prt(info,"recving cam %d",cam_index);
                    if(players.size()<cam_index)
                    {
                        //prt(info,"recving cam %d, our sz %d ",cam_index,players.size());
                        thread_lock.unlock();
                        continue;
                    }
                    PlayerWidget *w= players[cam_index-1];
                    w->set_output_data(rst.CameraOutput);
                    thread_lock.unlock();
                }else{
                    if(play_index==rst.CameraIndex)
                    {
                        PlayerWidget *w= players[0];
                        w->set_output_data(rst.CameraOutput);
                    }

                }
            }else{
                prt(info,"server output index %d,out of range(1- %d), make sure you\
                    loaded the server cfg & camera size >0 ",rst.CameraIndex,cfg.CameraData.size());
            }
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

    void request_get_config()
    {
        RequestPkt req= AppInputData::get_request(AppInputData::GET_CONFIG,0,JsonPacket());
        clt.send(QByteArray(req.data().str().data()));
    }
    void request_add_cam(int pos, CameraInputData data)
    {
        RequestPkt req= AppInputData::get_request(AppInputData::INSERT_CAMERA,pos,data.data());
        clt.send(QByteArray(req.data().str().data()));
    }
    void request_del_cam(int index)
    {
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
    void start_a_cam(int index)
    {
        thread_lock.lock();
        if(index>0&&index<=cfg.CameraData.size()){
            widget_append_camera(cfg.CameraData[index-1]);
        }else{
            prt(info,"cam %d out of range",index);
        }


        thread_lock.unlock();
    }
    void stop_config()
    {
        thread_lock.lock();
        for(PlayerWidget *w:players){
            widget_remove_camera(w);
            w->hide();
            //  delete w;//TODO: add deleting method
        }
        players.clear();
        thread_lock.unlock();
    }

    void camera_request(RequestPkt req,PlayerWidget *wgt)
    {
        int idx;
        if(play_mode==ALL_CAM)
            idx=std::find(players.begin(),players.end(),wgt)-players.begin();
        else
            idx=play_index;
        prt(info,"req from camera %d",idx+1);
        clt.send(get_request_pkt(AppInputData::MODIFY_CAMERA,idx+1,req.data()).data().str());
    }

    void on_pushButton_play_clicked();

    void on_pushButton_stop_clicked();

    void on_pushButton_clear_buffer_clicked();

    void on_pushButton_connect_clicked();

    void on_comboBox_play_index_activated(const QString &arg1);

    void on_comboBox_del_index_activated(int index);

    void on_checkBox_showoutput_clicked(bool checked);

    void on_checkBox_showinput_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    QUdpSocket *udpSocket;
    ServerInfoSearcher searcher;
    Client clt;
    vector<PlayerWidget*> players;
    AppInputData cfg;
    QMutex thread_lock;
    int play_mode;
    int play_index;
    int deleting_index;
};

#endif // MAINWINDOW_H
