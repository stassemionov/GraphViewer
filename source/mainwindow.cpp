#include "include/mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFileDialog>
#include <vector>

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
    menu_info->addAction("О программе",       this, SLOT(showInfo()));

    /*QLabel* l_minX = new QLabel("Min X");
    QLabel* l_maxX = new QLabel("Max X");
    QLabel* l_minY = new QLabel("Min Y");
    QLabel* l_maxY = new QLabel("Max Y");
    statusBar()->addWidget(l_minX);
    statusBar()->addWidget(l_maxX);
    statusBar()->addWidget(l_minY);
    statusBar()->addWidget(l_maxY);*/

    m_viewer = new Viewer(&m_points_info, this);
    ui->horizontalLayout->addWidget(m_viewer);
}

MainWindow::~MainWindow()
{
    delete m_viewer;
    delete ui;
}

/*void MainWindow::paintEvent(QPaintEvent *pEvent)
{

}

void MainWindow::resizeEvent(QResizeEvent *pEvent)
{
}*/


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
