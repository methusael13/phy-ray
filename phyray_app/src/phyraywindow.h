#ifndef PHYRAYWINDOW_H
#define PHYRAYWINDOW_H

#include <QMainWindow>

namespace Ui {
class PhyRayWindow;
}

class PhyRayWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PhyRayWindow(QWidget *parent = 0);
    ~PhyRayWindow();

private:
    Ui::PhyRayWindow *ui;
};

#endif // PHYRAYWINDOW_H
