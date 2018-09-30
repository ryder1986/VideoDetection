#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QLayout"
#include "playerwidget.h"
#include "camera.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{


   // DetectRegionInputData input=PlayerWidget::get_region_test_data( PlayerWidget::get_dummy_test_data().data(),LABLE_PROCESSOR_DUMMY);
 //   DetectRegionInputData input=PlayerWidget::get_region_test_data( PlayerWidget::get_c4_test_data().data(),LABLE_PROCESSOR_C4);
  //  DetectRegionInputData input=PlayerWidget::get_region_test_data( PlayerWidget::get_fvd_test_data().data(),LABLE_PROCESSOR_FVD);

    DetectRegionInputData input=PlayerWidget::get_region_test_data( PlayerWidget::get_mvd_test_data().data(),LABLE_PROCESSOR_MVD);

    vector <JsonPacket> rs;
    rs.push_back(input.data());
    //CameraInputData d(rs,"/root/test.264");
    CameraInputData d(rs,"rtsp://192.168.1.95:554/av0_1");
        wgt=new PlayerWidget(d);

    cam=(new Camera(d.data(),bind(&MainWindow::process_camera_data,
                                    this,placeholders::_1,
                                    placeholders::_2)));

    connect(wgt,SIGNAL(signal_camera(PlayerWidget*,int,JsonPacket)),this,SLOT(change_cfg(PlayerWidget*,int,JsonPacket)));
    ui->setupUi(this);
    ui->centralWidget->layout()->addWidget(wgt);
}

MainWindow::~MainWindow()
{
    delete ui;
}
