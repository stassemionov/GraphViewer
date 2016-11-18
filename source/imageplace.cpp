#include "include/imageplace.h"

#include <QPainter>

ImagePlace::ImagePlace(QPixmap* pixmap, QWidget *parent)
    : QWidget(parent), m_pixmap(pixmap)
{}

ImagePlace::~ImagePlace()
{
    delete m_pixmap;
}

void ImagePlace::resetPixmap(QPixmap* p)
{
    delete m_pixmap;
    m_pixmap = p;
}

void ImagePlace::paintEvent(QPaintEvent *pEvent)
{
    QPainter p;
    p.begin(this);
    p.drawPixmap(0, 0, *m_pixmap);
    p.end();
}

void ImagePlace::resizeEvent(QResizeEvent *pEvent)
{
    this->resetPixmap(new QPixmap(pEvent->size()));
    m_pixmap->fill(Qt::white);

    QPainter p;
    p.begin(m_pixmap);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setPen(QPen(Qt::black, 5, Qt::DashDotLine, Qt::RoundCap));
    p.setBrush(QBrush(Qt::white, Qt::SolidPattern));
    p.drawEllipse(QRect(QPoint(0,0),pEvent->size()));
    p.end();
}
