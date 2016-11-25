#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "viewer.h"

#include <QMainWindow>
#include <QTextEdit>
#include <QContextMenuEvent>
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

public slots:
    void editInputData();
    void showInfo();
    void specifyGridStep();
    void savePicture();
    void clearRecentList();
    void clearData();
    void savePoints();
    void onGraphUpdated();

protected slots:
    void openRecentFile();
    void openNewFile();
    void onPointInserted(const QPointF& point);
    void showMouseCoords();
    void hideMouseCoords();
    void updateMouseCoords(double x, double y);

protected:
    void openFile(const QString& fileName);
    void closeEvent(QCloseEvent *pEvent);
    void updateRecentList();

private:
    Viewer* m_viewer = nullptr;
    QTextEdit* m_text_edit = nullptr;
    QMenu* m_recent_files_menu = nullptr;
    QLabel* m_pos_label;

    QStringList m_recent_files;
    int m_max_recent_count = 10;
};

#endif // MAINWINDOW_H
