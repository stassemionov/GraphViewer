#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "viewer.h"

#include <QMainWindow>
#include <QTextEdit>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void updateGraph();

signals:

public slots:
    void openFile();
    void editInputData();
    void showInfo();
    void specifyGridStep();
//    void updatePointingMode();
    void savePicture();

protected:
    void resetData(const Points& points);
    void closeEvent(QCloseEvent *pEvent);

private:
    Viewer* m_viewer = nullptr;
    QTextEdit* m_text_edit = nullptr;
    PointsInfo m_points_info;
    double m_grid_step = -1;
//    bool m_pointing_mode = false;
};

#endif // MAINWINDOW_H
