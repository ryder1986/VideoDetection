#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    AppInputData input(cm.get_config());
    for(DetectionDevice dd:input.Devices){
        Client *c=new Client(dd.ip,dd.port);
        clts.push_back(c);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
 //   clt.send_msg();
    Client *c= clts[0];
    c->login();

}
