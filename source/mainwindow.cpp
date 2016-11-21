#include "include/mainwindow.h"
#include "include/dialogs.h"
#include "include/service.h"

#include <QFileDialog>
#include <QApplication>
#include <QDesktopWidget>
#include <QTime>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QLayout>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->setWindowIcon(QIcon(":/resources/icon.png"));
    this->setMinimumSize(300, 300);
    this->setGeometry(QApplication::desktop()->width()  / 4,
                      QApplication::desktop()->height() / 4,
                      QApplication::desktop()->width()  / 2,
                      QApplication::desktop()->height() / 2);

    QMenu* menu_file = menuBar()->addMenu("Файл");
    QMenu* menu_edit = menuBar()->addMenu("Правка");
    QMenu* menu_info = menuBar()->addMenu("Справка");

    menu_file->addAction("Открыть...",        this, SLOT(openFile()),
                         QKeySequence(Qt::CTRL + Qt::Key_O));
    menu_file->addAction("Выход",             this, SLOT(close()),
                         QKeySequence(Qt::CTRL + Qt::Key_Q));
    menu_edit->addAction("Входные данные...", this, SLOT(editInputData()),
                         QKeySequence(Qt::CTRL + Qt::Key_I));
    menu_edit->addAction("Шаг сетки...",      this, SLOT(specifyGridStep()),
                         QKeySequence(Qt::CTRL + Qt::Key_G));
    menu_info->addAction("О программе",       this, SLOT(showInfo()));

    m_viewer = new Viewer(&m_points_info, &m_grid_step);

    QFrame* frame = new QFrame;
    frame->setLayout(new QVBoxLayout);
    frame->layout()->addWidget(m_viewer);
    this->setCentralWidget(frame);

    m_text_edit = new QTextEdit;
    m_text_edit->setMaximumHeight(80);
    m_text_edit->setReadOnly(true);
    m_text_edit->append(QTime::currentTime().toString() +
                        QString(": Приветствие!"));
    m_text_edit->setFont(menuBar()->font());
    frame->layout()->addWidget(m_text_edit);

    this->statusBar()->hide();

    this->updateGraph();


}

MainWindow::~MainWindow()
{
    delete m_viewer;
    delete m_text_edit;
}

void MainWindow::updateGraph()
{
    if (m_points_info.getStoredCount() == 1)
    {
        m_text_edit->append(
            QTime::currentTime().toString() +
            QString(": Вывод отменен: недостаточно точек для масштабирования"));
    }

    m_viewer->repaint();
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

    QApplication::setOverrideCursor(Qt::WaitCursor);

    QTextStream stream(&file);
    Points data;
    bool success = readPoints(stream, data);
    file.close();

    if (!success)
    {
        m_text_edit->append(
            QTime::currentTime().toString() +
            QString(": Файл имеет ошибочный формат!"));
        QApplication::restoreOverrideCursor();
        return;
    }

    QApplication::restoreOverrideCursor();

    this->resetData(data);
    m_text_edit->append(
                QTime::currentTime().toString() +
        QString(": Данные загружены (") +
        QString::number(m_points_info.getStoredCount()) +
        QString(" точек)"));

    this->updateGraph();
}

void MainWindow::editInputData()
{
    DataEditionDialog dialog(m_points_info.getPoints());
    if (dialog.exec() == QDialog::Accepted)
    {
        Points data;
        bool success = dialog.getValue(data);

        if (!success)
        {
            m_text_edit->append(
                QTime::currentTime().toString() +
                QString(": Данные введены в ошибочном формате!"));
            return;
        }

        this->resetData(data);
        m_text_edit->append(
            QTime::currentTime().toString() +
            QString(": Данные приняты (") +
            QString::number(m_points_info.getStoredCount()) +
            QString(" точек)"));

        this->updateGraph();
    }
}

void MainWindow::showInfo()
{
    int w = 600;
    int h = 300;
    QWidget* info_widget = new QWidget;
    info_widget->setWindowTitle("О программе");
    info_widget->setFixedSize(w, h);
    info_widget->setGeometry(QApplication::desktop()->width()  / 2 - w/2,
                   QApplication::desktop()->height() / 2 - h/2,
                   w, h);

    QVBoxLayout* vlayout = new QVBoxLayout;
    QHBoxLayout* hlayout = new QHBoxLayout;
    QHBoxLayout* hlayout2 = new QHBoxLayout;
    vlayout->addLayout(hlayout);
    vlayout->addLayout(hlayout2);
    hlayout2->setAlignment(Qt::AlignRight);
    hlayout->setAlignment(Qt::AlignLeft);
    info_widget->setLayout(vlayout);
    QLabel* l = new QLabel;
    l->setPixmap(QPixmap(":/resources/icon.png").scaled(h/2, h/2));
    l->setFixedSize(h/2, h/2);
    hlayout->addWidget(l);
    QLabel* textlabel = new QLabel(loadInfo(":/resources/info.txt"));
  //  textlabel->setPixmap(QPixmap(":/resources/icon.png").scaled(100, 100));
    textlabel->setLayout(new QHBoxLayout);
    textlabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    textlabel->setFrameShape(QFrame::NoFrame);
    textlabel->setWordWrap(true);
    hlayout->addWidget(textlabel);
    QPushButton* button = new QPushButton("Закрыть");
    button->setMaximumWidth(w/6);
    connect(button, SIGNAL(clicked(bool)), info_widget, SLOT(close()));
    hlayout2->addWidget(button);
    info_widget->show();
}

void MainWindow::specifyGridStep()
{
    if (m_points_info.getStoredCount() == 0)
    {
        m_text_edit->append(
            QTime::currentTime().toString() +
            QString(": Чтобы указать шаг сетки, введите данные!"));
        return;
    }

    StepSpecifingDialog dialog(&m_grid_step);
    if (dialog.exec() == QDialog::Accepted)
    {
        if (dialog.getValue() != m_grid_step)
        {
            m_grid_step = dialog.getValue();
            m_text_edit->append(
                QTime::currentTime().toString() +
                QString("Шаг сетки установлен (") +
                QString::number(m_grid_step) + ")");
            this->updateGraph();
        }
    }
}

void MainWindow::resetData(const Points& points)
{
    m_points_info.setPoints(points);
    // By default, step equals 1/20 from minimum side of points rectangle.
    m_grid_step = qMax(m_points_info.getHDiff(),
                       m_points_info.getVDiff()) / 20;
}
