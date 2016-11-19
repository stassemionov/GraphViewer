#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QResizeEvent>
#include <QLabel>

#include "include/viewer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

public slots:
    void openFile();
    void editInputData();
    void showInfo();

protected:
    /*void paintEvent(QPaintEvent *pEvent);
    void resizeEvent(QResizeEvent *pEvent);*/

private:
    Ui::MainWindow *ui;
    Viewer* m_viewer;
    PointsInfo m_points_info;
};

#endif // MAINWINDOW_H
