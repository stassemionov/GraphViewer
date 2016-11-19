#include "include/viewer.h"

#include <QPainter>
#include <QDebug>

Viewer::Viewer(const PointsInfo *points_info, QWidget *parent)
    : QWidget(parent),
      m_pixmap(new QPixmap(this->size())),
      m_points_info(points_info)
{}

Viewer::~Viewer()
{
    delete m_pixmap;
}

void inline Viewer::resetPixmap(QPixmap* p)
{
    delete m_pixmap;
    m_pixmap = p;
}

void Viewer::resetPoints(const PointsInfo& points_info)
{
    m_points_info = &points_info;
    repaint();
}

void Viewer::formPixmap()
{
    m_pixmap->fill(Qt::white);

    int web_step = 40;

    QPainter painter;
    painter.begin(m_pixmap);
    painter.setPen(QPen(Qt::gray, 1));
    for (int i = this->height()-1; i > -1; i -= web_step)
    {
        painter.drawLine(0, i, this->width(), i);
    }
    for (int i = 0; i < this->width(); i += web_step)
    {
        painter.drawLine(i, 0, i, this->height());
    }
    painter.end();

    // Sizes of work area.
    int wx = qMin(this->width(), this->height()) - 100;
  //  int wy = 0.95 * this->height();
    int dx = (qMin(this->width(), this->height()) - wx) / 2;
  //  int dy = (this->height() - wy) / 2;

    painter.begin(m_pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, 2/*, Qt::DashDotLine, Qt::RoundCap*/));
    painter.setFont(QFont("Times New Roman",10));
  //  p.setBrush(QBrush(Qt::white, Qt::SolidPattern));

    const Points& points = m_points_info->getPoints();
    for (Points::const_iterator it = points.begin();
         it != points.end(); ++it)
    {
        qreal x = dx + (it->x() - m_points_info->getLeftBound()) /
                m_points_info->getHDiff() * wx;
        qreal y = dx + (it->y() - m_points_info->getLowerBound()) /
                m_points_info->getVDiff() * wx;
        QPointF point(x,this->height()-y);

        painter.drawEllipse(point, 3, 3);
        painter.drawEllipse(point, 6, 6);
        painter.drawText(x+10, point.y()+3,
            "[" + QString::number(it->x()) + ", " + QString::number(it->y()) + "]");
    }

    QFontMetrics metrics(QFont("Times New Roman", 12));

    QString str1 = "Min X = " + QString::number(m_points_info->getLeftBound());
    QString str2 = "Max X = " + QString::number(m_points_info->getRightBound());
    QString str3 = "Min Y = " + QString::number(m_points_info->getLowerBound());
    QString str4 = "Max Y = " + QString::number(m_points_info->getUpperBound());

    painter.setPen(QPen(Qt::darkRed, 2));
    painter.setFont(QFont("Times New Roman",12));

    painter.drawText(web_step, this->height() - 2, str1);
    painter.drawText(this->width() - (metrics.width(str2) + 2), this->height() - 2, str2);

    painter.rotate(90);
    painter.drawText(this->height() - metrics.width(str3) - web_step, -2, str3);
    painter.drawText(2, -2, str4);
    painter.rotate(-90);

    painter.end();
}

void Viewer::paintEvent(QPaintEvent *pEvent)
{
    if (m_points_info->getStoredCount() == 0)
    {
        return;
    }

    this->formPixmap();

    QPainter p;
    p.begin(this);
    p.drawPixmap(0, 0, *m_pixmap);
    p.end();
}

void Viewer::resizeEvent(QResizeEvent *pEvent)
{
    this->resetPixmap(new QPixmap(this->size()));
}
