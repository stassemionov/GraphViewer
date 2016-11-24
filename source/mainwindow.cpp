#include "mainwindow.h"
#include "dialogs.h"
#include "service.h"

#include <QFileDialog>
#include <QApplication>
#include <QDesktopWidget>
#include <QTime>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QLayout>
#include <QStatusBar>
#include <QSettings>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    qRegisterMetaTypeStreamOperators<QList<QPointF> >("QList<QPointF>");

    this->setWindowIcon(QIcon(":/resources/icon.png"));
    this->setMinimumSize(400, 400);

    QSettings settings(QCoreApplication::organizationName(), "GraphViewer");
    if (settings.contains("geometry"))
    {
        restoreGeometry(settings.value("geometry").toByteArray());
    }
    else
    {
        this->setGeometry(QApplication::desktop()->width()  / 4,
                          QApplication::desktop()->height() / 4,
                          QApplication::desktop()->width()  / 2,
                          QApplication::desktop()->height() / 2);
    }
    this->resetData(settings.value("points").value<Points>());
    m_recent_files = settings.value("recent").toStringList();

    QMenu* menu_file = menuBar()->addMenu(QString::fromLocal8Bit("Файл"));
    QMenu* menu_edit = menuBar()->addMenu(QString::fromLocal8Bit("Правка"));
//    QMenu* menu_tool = menuBar()->addMenu(QString::fromLocal8Bit("Инструменты"));
    QMenu* menu_info = menuBar()->addMenu(QString::fromLocal8Bit("Справка"));

    menu_file->addAction(QString::fromLocal8Bit("Открыть..."),
                         this, SLOT(openNewFile()),
                         QKeySequence(Qt::CTRL + Qt::Key_O));
    m_recent_files_menu = menu_file->addMenu(
                QString::fromLocal8Bit("Недавние..."));
    menu_file->addSeparator();
    menu_file->addAction(QString::fromLocal8Bit("Сохранить данные..."),
                         this, SLOT(savePoints()),
                         QKeySequence(Qt::CTRL + Qt::Key_S));
    menu_file->addAction(QString::fromLocal8Bit("Сохранить график..."),
                         this, SLOT(savePicture()),
                         QKeySequence(Qt::CTRL + Qt::Key_P));
    menu_file->addSeparator();
    menu_file->addAction(QString::fromLocal8Bit("Выход"),
                         this, SLOT(close()),
                         QKeySequence(Qt::CTRL + Qt::Key_Q));
    menu_edit->addAction(QString::fromLocal8Bit("Входные данные..."),
                         this, SLOT(editInputData()),
                         QKeySequence(Qt::CTRL + Qt::Key_I));
    menu_edit->addAction(QString::fromLocal8Bit("Шаг сетки..."),
                         this, SLOT(specifyGridStep()),
                         QKeySequence(Qt::CTRL + Qt::Key_G));
    menu_edit->addSeparator();
    menu_edit->addAction(QString::fromLocal8Bit("Очистить график"),
                         this, SLOT(clearData()));
    menu_info->addAction(QString::fromLocal8Bit("О программе"),
                         this, SLOT(showInfo()));
    //menu_tool->addAction(QString::fromLocal8Bit("Указать новую точку"),
    //                     this, SLOT(updatePointingMode()),
    //                     QKeySequence(Qt::CTRL + Qt::Key_N));

    m_recent_files_menu->actions().reserve(m_max_recent_count + 2);
    for (int i = 0; i < m_max_recent_count; ++i)
    {
        QAction* action = new QAction(m_recent_files_menu);
        connect(action, SIGNAL(triggered()), this, SLOT(openRecentFile()));
        m_recent_files_menu->addAction(action);
    }
    QAction* clear_action = new QAction(
                QString::fromLocal8Bit("Очистить список"),
                m_recent_files_menu);
    connect(clear_action, SIGNAL(triggered()), this, SLOT(clearRecentList()));
    m_recent_files_menu->addSeparator();
    m_recent_files_menu->addAction(clear_action);

    this->updateRecentList();

    m_viewer = new Viewer(&m_points_info, &m_grid_step);
    connect(m_viewer, SIGNAL(onPointInserted(const QPointF&)),
            this, SLOT(pointInsertion(const QPointF&)));

    QFrame* frame = new QFrame;
    frame->setLayout(new QVBoxLayout);
    frame->layout()->addWidget(m_viewer);
    this->setCentralWidget(frame);

    m_text_edit = new QTextEdit;
    m_text_edit->setMaximumHeight(80);
    m_text_edit->setReadOnly(true);
    m_text_edit->append(QTime::currentTime().toString() +
                        QString::fromLocal8Bit(": Приветствие!"));
    m_text_edit->setFont(menuBar()->font());
    frame->layout()->addWidget(m_text_edit);

    this->statusBar()->hide();

    this->updateGraph();
}

MainWindow::~MainWindow()
{
    delete m_viewer;
    delete m_text_edit;
    delete m_recent_files_menu;
}

void MainWindow::updateRecentList()
{
    QMutableStringListIterator it(m_recent_files);
    while (it.hasNext())
    {
        if (!QFile::exists(it.next()))
        {
            it.remove();
        }
    }

    while (m_recent_files.size() > m_max_recent_count)
    {
        m_recent_files.pop_back();
    }

    QList<QAction*> actions = m_recent_files_menu->actions();
    for (int i = 0; i < actions.size() - 2; ++i)
    {
        if (i < m_recent_files.size())
        {
            actions[i]->setText(m_recent_files[i]);
            actions[i]->setVisible(true);
        }
        else
        {
            actions[i]->setVisible(false);
        }
    }

    actions.back()->setVisible(!m_recent_files.empty());
}

void MainWindow::updateGraph()
{
    if (m_points_info.getStoredCount() == 1)
    {
        m_text_edit->append(
            QTime::currentTime().toString() +
            QString::fromLocal8Bit(": Вывод отменен: недостаточно точек для масштабирования"));
    }

    m_viewer->repaint();
}

void MainWindow::openNewFile()
{
    QString name = QFileDialog::getOpenFileName(this,
                QString::fromLocal8Bit("Открыть файл"),
                QString(), QString("Text (*.txt)"));
    openFile(name);
    if (!m_recent_files.contains(name))
    {
        m_recent_files.push_front(name);
        this->updateRecentList();
    }
}

void MainWindow::openRecentFile()
{
    QString name = dynamic_cast<QAction*>(sender())->text();
    openFile(name);
}

void MainWindow::openFile(const QString& fileName)
{
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
            QString::fromLocal8Bit(": Файл имеет ошибочный формат!"));
        QApplication::restoreOverrideCursor();
        return;
    }

    QApplication::restoreOverrideCursor();

    this->resetData(data);
    m_text_edit->append(
        QString::fromLocal8Bit("%1: Данные загружены (%2 точек)").
                arg(QTime::currentTime().toString()).
                arg(m_points_info.getStoredCount()));

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
                QString::fromLocal8Bit(": Данные введены в ошибочном формате!"));
            return;
        }

        this->resetData(data);

        m_text_edit->append(
            QString::fromLocal8Bit("%1: Данные загружены (%2 точек)").
                    arg(QTime::currentTime().toString()).
                    arg(m_points_info.getStoredCount()));

        this->updateGraph();
    }
}

void MainWindow::showInfo()
{
    int w = 600;
    int h = 300;
    QWidget* info_widget = new QWidget;
    info_widget->setWindowTitle(QString::fromLocal8Bit("О программе"));
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
    textlabel->setLayout(new QHBoxLayout);
    textlabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    textlabel->setFrameShape(QFrame::NoFrame);
    textlabel->setWordWrap(true);
    hlayout->addWidget(textlabel);
    QPushButton* button = new QPushButton(QString::fromLocal8Bit("Закрыть"));
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
            QString::fromLocal8Bit(": Чтобы указать шаг сетки, введите данные!"));
        return;
    }

    StepSpecifingDialog dialog(&m_grid_step);
    if (dialog.exec() == QDialog::Accepted)
    {
        if (dialog.getValue() != m_grid_step)
        {
            m_grid_step = dialog.getValue();
            m_text_edit->append(
                QString::fromLocal8Bit("%1: Шаг сетки установлен (%2)").
                        arg(QTime::currentTime().toString()).
                        arg(m_grid_step));
            this->updateGraph();
        }
    }
}

void MainWindow::resetData(const Points& points)
{
    if (points.empty())
    {
        return;
    }

    m_points_info.setPoints(points);
    // By default, step equals 1/20 from minimum side of points rectangle.
    m_grid_step = qMax(m_points_info.getHDiff(),
                       m_points_info.getVDiff()) / 20;
}

void MainWindow::savePicture()
{
    if (m_points_info.getStoredCount() < 2)
    {
        m_text_edit->append(
            QString::fromLocal8Bit("%1: Изображение отсутствует").
                    arg(QTime::currentTime().toString()));
    }
    else
    {
        QFileDialog dial;
        const QString dir = dial.directory().path();

        QString fileName = QFileDialog::getSaveFileName(
                    this,
                    QString::fromLocal8Bit("Сохранить график"),
                    dir + QString("/file.jpg"),
                    QString("Images (*.png *.jpg)"));
        if (fileName.isEmpty())
        {
            return;
        }
        m_viewer->getPixmap()->save(fileName, "JPEG");
        m_text_edit->append(
            QString::fromLocal8Bit("%1: Изображение сохранено по адресу: %2").
                    arg(QTime::currentTime().toString()).
                    arg(fileName));
    }
}

void MainWindow::savePoints()
{
    if (m_points_info.getStoredCount() == 0)
    {
        m_text_edit->append(
            QString::fromLocal8Bit("%1: Данные не заданы!").
                    arg(QTime::currentTime().toString()));
    }
    else
    {
        QString fileName = QFileDialog::getSaveFileName(
                    this,
                    QString::fromLocal8Bit("Сохранить данные"),
                    QString(), QString("Text (*.txt)"));
        if (fileName.isEmpty())
        {
            return;
        }

        QFile file(fileName);
        //if (file.exists())
        //{
        //    m_text_edit->append(
        //        QString::fromLocal8Bit("%1: Файл уже существует: %2").
        //                arg(QTime::currentTime().toString()).
        //                arg(fileName));
        //    return;
        //}
        file.open(QIODevice::WriteOnly);
        QString data_string(convertToString(m_points_info.getPoints()));
        QTextStream stream(&file);
        stream << data_string;
        file.close();

        m_text_edit->append(
            QString::fromLocal8Bit("%1: Данные сохранены по адресу: %2").
                    arg(QTime::currentTime().toString()).
                    arg(fileName));
    }
}

void MainWindow::pointInsertion(const QPointF& point)
{
    m_text_edit->append(
        QString::fromLocal8Bit("%1: Точка добавлена: [%2,%3]").
                arg(QTime::currentTime().toString()).
                arg(point.x()).
                arg(point.y()));

    this->updateGraph();
}

void MainWindow::clearRecentList()
{
    m_recent_files.clear();
    this->updateRecentList();
}

void MainWindow::clearData()
{
    m_points_info.clear();
    this->updateGraph();
}

void MainWindow::closeEvent(QCloseEvent *pEvent)
{
    pEvent->accept();
    QSettings settings(QCoreApplication::organizationName(), "GraphViewer");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("points", QVariant::fromValue(m_points_info.getPoints()));
    settings.setValue("recent", m_recent_files);
}
