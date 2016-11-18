#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QResizeEvent>

#include "include/imageplace.h"

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
    ImagePlace* m_img_place;
};

#endif // MAINWINDOW_H
