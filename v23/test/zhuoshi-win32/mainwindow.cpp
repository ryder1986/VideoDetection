#include "mainwindow.h"
#include "ui_mainwindow.h"
 string Database::ip;
 string Database::dbname;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    JsonPacket pkt=cm.get_config();
    prt(info,"%s",pkt.str().data());
    AppInputData input(cm.get_config());
    Database::ip=input.ip;
    Database::dbname=input.name;

    for(DetectionDevice dd:input.Devices){
        DetectorConnection *c=new DetectorConnection(dd.ip,dd.port);
        detectors.push_back(c);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
 //   clt.send_msg();
//    Client *c= clts[0];
//    c->login();

}
