#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "videoprocessor.h".h"
#include "detectregion_data.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&searcher,SIGNAL(find_ip(QString)),this,SLOT(search_server_result(QString)));
    connect(&clt,SIGNAL(server_msg(QString)),this,SLOT(server_msg(QString)));
    start_udp();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::search_server_result(QString ip)
{
    ui->comboBox_search->addItem(ip);
}

void MainWindow::on_pushButton_search_clicked()
{
    ui->comboBox_search->clear();
    searcher.search_device();
}

void MainWindow::on_comboBox_search_activated(const QString &arg1)
{
     clt.connect_to_server(arg1);
}
//#include "mvdprocessor.h"
void MainWindow::on_pushButton_addcam_clicked()
{
    DummyProcessorInputData di=DummyProcessorInputData::get_dummy_test_data();
    DetectRegionInputData dri=DetectRegionInputData::get_region_test_data(di.data(),LABEL_PROCESSOR_DUMMY);
    vector<DetectRegionInputData> dris;
    dris.push_back(dri);
   // CameraInputData  d=CameraInputData::get_camera_test_data(dris,"/root/test.mp4");
    //CameraInputData  d=CameraInputData::get_camera_test_data(dris,"D:/build/test.mp4");
    CameraInputData  d=CameraInputData::get_camera_test_data(dris,ui->lineEdit_addcam->text().toStdString());
    request_add_cam(cfg.CameraData.size(),d);
    //insert_into_vector(players.size(),players,new PlayerWidget(d));
    //ui->gridLayout_video->addWidget(players.back());
}

void MainWindow::on_pushButton_delcam_clicked()
{
    int del_index=ui->lineEdit_delcam->text().toInt();
    request_del_cam(del_index);
    //del_from_vector(del_index,players);
}

void MainWindow::on_pushButton_load_clicked()
{
    request_get_config();
}

void MainWindow::server_msg(QString msg)
{
    //ui->plainTextEdit_recive->setPlainText(msg);//show what we got
    string str(msg.toUtf8());
    ui->textEdit_netbuffer->setPlainText(msg);
    ReplyPkt event(str);
    switch(event.Operation){
    case AppInputData::Operation::GET_CONFIG:
    {
        cfg=event.Data;
        prt(info,"%s",cfg.data().str().data());
        //            stop_config();
        //            start_config();
        break;
    }
    case AppInputData::Operation::INSERT_CAMERA:
    {
        request_get_config();
        break;
    }
    default:break;
    }
}

void MainWindow::widget_append_camera(CameraInputData d)
{

    // ui->ccomboBox_cameras->addItem(d.Url.data());
    PlayerWidget *player=new PlayerWidget(d);
    connect(player,SIGNAL(camera_request(RequestPkt,PlayerWidget *)),this,SLOT(camera_request(RequestPkt,PlayerWidget*)));
    players.push_back(player);
    ui->gridLayout_video->addWidget(player);
    //    connect(player,SIGNAL(cam_data_change(CameraInputData,QWidget*)),\
    //            this,SLOT(generate_current_config(CameraInputData,QWidget*)));

    //    connect(player,SIGNAL(signal_camera(PlayerWidget*,int,JsonPacket)),\
    //            this,SLOT(slot_camera(PlayerWidget*,int,JsonPacket)));

//    connect(player,SIGNAL(click_event(PlayerWidget *,int)),\
//            this,SLOT(player_event(PlayerWidget*,int)));
}
void MainWindow::widget_remove_camera(QWidget *w)
{
    ui->gridLayout_video->removeWidget(w);
}

void MainWindow::on_pushButton_play_clicked()
{
    stop_config();
    start_config();
}

void MainWindow::on_pushButton_stop_clicked()
{
    stop_config();
}

void MainWindow::on_pushButton_clear_buffer_clicked()
{
    ui->textEdit_netbuffer->clear();
}
