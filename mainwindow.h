#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QPixmap>
#include <QResizeEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void openFile();

protected:
    void paintEvent(QPaintEvent *pEvent);
    void resizeEvent(QResizeEvent *pEvent);

private:
    Ui::MainWindow *ui;
    QPixmap* m_pixmap;
    QPainter* m_painter;
};

#endif // MAINWINDOW_H
