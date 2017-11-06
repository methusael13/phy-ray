#include "phyraywindow.h"
#include "ui_phyraywindow.h"

PhyRayWindow::PhyRayWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PhyRayWindow)
{
    ui->setupUi(this);
}

PhyRayWindow::~PhyRayWindow()
{
    delete ui;
}
