#include <phyraywindow.h>

namespace Ui {

PhyRayWindow::PhyRayWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::PhyRayWindow) {
    ui->setupUi(this);
}

PhyRayWindow::~PhyRayWindow() {
    delete ui;
}

}  // namespace Ui
