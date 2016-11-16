#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QMenu* menu = ui->menuBar->addMenu("Input data...");
    QAction *action1 = menu->addAction(
        QIcon(":/menu/resources/open.png"),
        "Open file",
        this,
        SLOT(open()));



    m_painter = new QPainter(this);
    m_pixmap = new QPixmap(this->size());

 //   ui->label->setPixmap(*m_pixmap);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *pEvent)
{
    m_painter->begin(this);
    m_painter->drawPixmap(0, 0, *m_pixmap);
    m_painter->end();
}

void MainWindow::resizeEvent(QResizeEvent *pEvent)
{
    delete m_pixmap;
    m_pixmap = new QPixmap(pEvent->size());
    m_pixmap->fill(Qt::white);

    m_painter->begin(m_pixmap);
    m_painter->setRenderHint(QPainter::Antialiasing, true);
    m_painter->setPen(QPen(Qt::black, 12, Qt::DashDotLine, Qt::RoundCap));
    m_painter->setBrush(QBrush(Qt::white, Qt::SolidPattern));
    m_painter->drawEllipse(QRect(QPoint(0,0),pEvent->size()));
    m_painter->end();
}


void MainWindow::openFile()
{

}
