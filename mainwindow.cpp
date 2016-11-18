#include "mainwindow.h"
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

    QMenu* menu1 = ui->menuBar->addMenu("Файл");
    QMenu* menu2 = ui->menuBar->addMenu("Правка");
    QMenu* menu3 = ui->menuBar->addMenu("Сравка");

    QAction *action1 = menu1->addAction(
        "Открыть",
        this,
        SLOT(openFile()));
    QAction *action2 = menu1->addAction(
                "Выход",
                this,
                SLOT(close()));
    QAction *action3 = menu2->addAction(
        "Входные данные...",
        this,
        SLOT(editInputData()));
    QAction *action4 = menu3->addAction(
        "О программе",
        this,
        SLOT(showInfo()));

    m_img_place = new ImagePlace(new QPixmap(this->size()), this);
    ui->horizontalLayout->addWidget(m_img_place);
}

MainWindow::~MainWindow()
{
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

    QTextStream stream(&file);
    QList<QPair<double,double> > points;

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
            points.push_back(QPair<double,double>(str1.toDouble(), str2.toDouble()));
        }
    }

    file.close();
}

void MainWindow::editInputData()
{

}


void MainWindow::showInfo()
{

}

/*
void MainWindow::loadFile(const QString &rcFileName)
{
    QPixmap pixmap;
    QApplication::setOverrideCursor(Qt::WaitCursor);
    const bool cbSuccess = pixmap.load(rcFileName);
    QApplication::restoreOverrideCursor();
    if (cbSuccess)
    {
        m_pLabelImage->setPixmap(pixmap);
        m_sizeImage = pixmap.rect().size();
        m_fileName = rcFileName;
        m_listRecentFiles.removeAll(m_fileName);
        m_listRecentFiles.prepend(m_fileName);
        updateRecentFileActions();
        updateStatusBar();
        statusBar()->showMessage(tr("File loaded"), 2000);
    }
}
*/
