#ifndef PHYRAYWINDOW_H
#define PHYRAYWINDOW_H

#include <QMainWindow>

#include <ui_phyraywindow.h>

namespace Ui {

class PhyRayWindow : public QMainWindow, Ui_PhyRayWindow {
    Q_OBJECT

public:
    explicit PhyRayWindow(QWidget *parent = 0);
    ~PhyRayWindow();

private:
    Ui::PhyRayWindow *ui;
};

}  // namespace Ui

#endif
