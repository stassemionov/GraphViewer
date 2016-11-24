#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "viewer.h"

#include <QMainWindow>
#include <QTextEdit>
#include <QContextMenuEvent>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void updateGraph();

signals:

public slots:
    void openRecentFile();
    void openNewFile();
    void editInputData();
    void showInfo();
    void specifyGridStep();
//    void updatePointingMode();
    void savePicture();
    void clearRecentList();
    void clearData();
    void savePoints();

protected slots:
    void pointInsertion(const QPointF& point);

protected:
    void openFile(const QString& fileName);
    void resetData(const Points& points);
    void closeEvent(QCloseEvent *pEvent);
    void updateRecentList();

private:
    Viewer* m_viewer = nullptr;
    QTextEdit* m_text_edit = nullptr;
    QMenu* m_recent_files_menu = nullptr;
    PointsInfo m_points_info;
    double m_grid_step = -1;
    QStringList m_recent_files;
    int m_max_recent_count = 10;
//    bool m_pointing_mode = false;
};

#endif // MAINWINDOW_H
