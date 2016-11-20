#include "include/viewer.h"

#include <QPainter>
#include <QDebug>

Viewer::Viewer(const PointsInfo *points_info, const double *step, QWidget *parent)
    : QWidget(parent),
      m_pixmap(new QPixmap(this->size())),
      m_points_info(points_info),
      m_grid_step(step)
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

    // Parameteres of work area.
    const int w = this->width();
    const int h = this->height();
    const int dx = 30;

    const int wsize = qMin(w, h);
    const qreal hdiff = m_points_info->getHDiff();
    const qreal vdiff = m_points_info->getVDiff();
    // Scale of picture - count of pixels by unit of space.
    const double scale = (wsize == h) ? (h-2*dx) / vdiff : (w-2*dx) / hdiff;
    const int step = (*m_grid_step) * scale;
    const int wx = wsize - 2 * dx;
    const qreal hdiff_full = w / scale;
    const qreal vdiff_full = h / scale;

    QPainter painter;
    painter.begin(m_pixmap);
    painter.setPen(QPen(Qt::gray, 0.5));

    // Grid drawing.
    double i = vdiff_full - dx/scale, j = dx/scale;
    while (i >= 0 )
    {
        painter.drawLine(0, i*scale, w, i*scale);
        i -= *m_grid_step;
    }
    i = vdiff_full - dx/scale + *m_grid_step;
    while (i <= vdiff_full )
    {
        painter.drawLine(0, i*scale, w, i*scale);
        i += *m_grid_step;
    }
    while (j < hdiff_full)
    {
        painter.drawLine(j*scale, 0, j*scale, h);
        j += *m_grid_step;
    }
    j = dx/scale - *m_grid_step;
    while (j >= 0)
    {
        painter.drawLine(j*scale, 0, j*scale, h);
        j -= *m_grid_step;
    }
    painter.end();

    painter.begin(m_pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, 2));
    painter.setFont(QFont("Times New Roman", 10));
  //  p.setBrush(QBrush(Qt::white, Qt::SolidPattern));

    const Points& points = m_points_info->getPoints();
    const qreal lb = m_points_info->getLeftBound();
    const qreal lwb = m_points_info->getLowerBound();
    for (Points::const_iterator it = points.begin();
         it != points.end(); ++it)
    {
       // qreal x = dx + (it->x() - lb) / hdiff * wx;
       // qreal y = dx + (it->y() - lwb) / vdiff * wx;
        qreal x = dx + (it->x() - lb) * scale;
        qreal y = dx + (it->y() - lwb) * scale;
        QPointF point(x, h - y);

        painter.drawEllipse(point, 3, 3);
        painter.drawEllipse(point, 6, 6);
        painter.drawText(x + 10, point.y() + 3,
            "[" + QString::number(it->x()) + ", " + QString::number(it->y()) + "]");
    }

    const qreal ub_coord = m_points_info->getVDiff() * scale;
    const qreal rb_coord = m_points_info->getHDiff() * scale;
    QFontMetrics metrics(QFont("Times New Roman", 12));

    QString str_minX = "Min X = " + QString::number(lb);
    QString str_maxX = "Max X = " + QString::number(m_points_info->getRightBound());
    QString str_minY = "Min Y = " + QString::number(lwb);
    QString str_maxY = "Max Y = " + QString::number(m_points_info->getUpperBound());

    painter.setPen(QPen(Qt::darkRed, 2));
    painter.setFont(QFont("Times New Roman", 12));

    painter.drawText(dx, h - 3, str_minX);
    painter.drawText(dx + rb_coord - metrics.width(str_maxX), h - 3, str_maxX);

    painter.rotate(90);
    painter.drawText(h - metrics.width(str_minY) - dx, -3, str_minY);
    painter.drawText(h - (dx + ub_coord), -3, str_maxY);
    painter.rotate(-90);

    // Axis drawing.
    painter.setPen(QPen(Qt::darkRed, 2));
    painter.drawLine(1, 0, 1, h - 1);
    painter.drawLine(0, h - 1, w-1, h-1);

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
