#include "include/mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFileDialog>

using std::vector;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QMenu* menu_file = menuBar()->addMenu("Файл");
    QMenu* menu_edit = menuBar()->addMenu("Правка");
    QMenu* menu_info = menuBar()->addMenu("Справка");

    menu_file->addAction("Открыть...",        this, SLOT(openFile()));
    menu_file->addAction("Выход",             this, SLOT(close()));
    menu_edit->addAction("Входные данные...", this, SLOT(editInputData()));
    menu_edit->addAction("Шаг сетки...",      this, SLOT(specifyGridStep()));
    menu_info->addAction("О программе",       this, SLOT(showInfo()));

    m_grid_step = -1;
    m_viewer = new Viewer(&m_points_info, &m_grid_step, this);
    ui->horizontalLayout->addWidget(m_viewer);
}

MainWindow::~MainWindow()
{
    delete m_viewer;
    delete ui;
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (fileName.isEmpty())
    {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }

    m_points_info.clear();

    QApplication::setOverrideCursor(Qt::WaitCursor);

    QTextStream stream(&file);
    while (!stream.atEnd())
    {
        QString str1, str2;
        while (str1.isEmpty() && !stream.atEnd())
        {
            stream >> str1;
        }
        while (str2.isEmpty() && !stream.atEnd())
        {
            stream >> str2;
        }
        if (!str1.isEmpty() && !str2.isEmpty())
        {
            m_points_info.addPoint(
                QPointF(str1.toDouble(), str2.toDouble()));
        }
    }

    // By dedfault, step equals 1/20 from minimum side of points rectangle.
    m_grid_step = qMin(m_points_info.getHDiff(), m_points_info.getHDiff()) / 20;

    QApplication::restoreOverrideCursor();

    statusBar()->showMessage(
        QString("Данные загружены (") +
        QString::number(m_points_info.getStoredCount()) +
        QString(" точек)"), 3000);

    file.close();

    repaint();
}

void MainWindow::editInputData()
{

}

void MainWindow::showInfo()
{

}

void MainWindow::specifyGridStep()
{
    if (m_points_info.getStoredCount() == 0)
    {
        statusBar()->showMessage(
            QString("Данные для отображения не определены!"), 3000);
        return;
    }

    StepSpecifingDialog dialog(&m_grid_step);
    if (dialog.exec() == QDialog::Accepted)
    {
        if (dialog.getValue() != m_grid_step)
        {
            m_grid_step = dialog.getValue();
            statusBar()->showMessage(
                QString("Шаг сетки установлен (") +
                QString::number(m_grid_step) +
                ")", 3000);
            this->repaint();
        }
    }
}


// *** StepSpecifingDialog class implementation *** //

StepSpecifingDialog::StepSpecifingDialog(const double *step_init, QWidget *parent)
    : QDialog(parent), m_current_step(step_init)
{
    this->setWindowTitle("Установка шага сетки");
    this->setFixedSize(QSize(250, 100));
    this->setModal(true);
    QVBoxLayout* layout = new QVBoxLayout;
    this->setLayout(layout);
    m_spin_box = new QDoubleSpinBox;
    m_spin_box->setMinimum(0.0);
    m_spin_box->setValue(*m_current_step);
    layout->addWidget(m_spin_box);
    m_button = new QPushButton("OK");
    layout->addWidget(m_button);
    connect(m_button, SIGNAL(clicked(bool)), this, SLOT(accept()));
}

StepSpecifingDialog::~StepSpecifingDialog()
{
    delete m_spin_box;
    delete m_button;
}

double StepSpecifingDialog::getValue() const
{
    return m_spin_box->value();
}
