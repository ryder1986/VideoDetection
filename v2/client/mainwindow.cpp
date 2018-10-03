#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&searcher,SIGNAL(find_ip(QString)),this,SLOT(search_server_result(QString)));
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

void MainWindow::on_pushButton_addcam_clicked()
{
    insert_into_vector(players.size(),players,new PlayerWidget());
    ui->gridLayout_video->addWidget(players.back());
}

void MainWindow::on_pushButton_delcam_clicked()
{
    int del_index=ui->lineEdit_delcam->text().toInt();
    del_from_vector(del_index,players);
}
