#include "activex.h"
#include "ui_activex.h"

ActiveX::ActiveX(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ActiveX)
{
    ui->setupUi(this);
}

ActiveX::~ActiveX()
{
    delete ui;
}

QString ActiveX::Version()
{
    return QString("ActiveX 1.0.0");
}
