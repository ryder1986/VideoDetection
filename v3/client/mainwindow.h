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
            prt(info,"get data %s",sz);
//            QString str(datagram.data());
//            JsonPacket pkt(str.toStdString());
//            AppOutputData rst( pkt  );

//            int cam_index=rst.CameraIndex;
//            //    CameraInputData camera_cfg=cfg.CameraData[cam_index];

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

    void on_pushButton_search_clicked();

    void on_comboBox_search_activated(const QString &arg1);

    void on_pushButton_addcam_clicked();

    void on_pushButton_delcam_clicked();

    void on_pushButton_load_clicked();
    void get_config()
    {
        clt.get_config();
    }
private:
    Ui::MainWindow *ui;
    QUdpSocket *udpSocket;
    ServerInfoSearcher searcher;
    Client clt;
    vector<PlayerWidget*> players;
};

#endif // MAINWINDOW_H
