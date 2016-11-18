#ifndef IMAGEPLACE_H
#define IMAGEPLACE_H

#include <QWidget>
#include <QPixmap>
#include <QPaintEvent>
#include <QResizeEvent>

class ImagePlace : public QWidget
{
    Q_OBJECT

public:
    explicit ImagePlace(QPixmap* pixmap, QWidget *parent = nullptr);

    ~ImagePlace();

    void resetPixmap(QPixmap* p);

signals:

protected:
    void paintEvent(QPaintEvent *pEvent);
    void resizeEvent(QResizeEvent *pEvent);

private:
    QPixmap* m_pixmap;
};

#endif // IMAGEPLACE_H
