#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "playerwidget.h"
#include "camera.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void change_cfg(PlayerWidget*,int,JsonPacket pkt)
    {
        cam->modify(pkt);
    }
private:
    void process_camera_data(Camera *camera, string data)
    {
        wgt->set_overlay(data);
    }
    Ui::MainWindow *ui;
    PlayerWidget *wgt;
    Camera *cam;
};

#endif // MAINWINDOW_H
