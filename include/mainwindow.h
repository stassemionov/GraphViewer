#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QResizeEvent>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QDialog>
#include <QVBoxLayout>

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
    void specifyGridStep();

private:
    Ui::MainWindow *ui;
    Viewer* m_viewer;
    PointsInfo m_points_info;
    double m_grid_step;
};

class StepSpecifingDialog : public QDialog
{

    Q_OBJECT

public:
    explicit StepSpecifingDialog(const double* step_init, QWidget *parent = nullptr);

    ~StepSpecifingDialog();

    double getValue() const;

private:
    QDoubleSpinBox* m_spin_box = nullptr;
    QPushButton* m_button = nullptr;
    const double* m_current_step = nullptr;
};

#endif // MAINWINDOW_H
