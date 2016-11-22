#include "viewer.h"

#include <QPainter>
#include <QApplication>
#include <QDebug>

Viewer::Viewer(const PointsInfo *points_info, const double *step, QWidget *parent)
    : QWidget(parent),
      m_pixmap(new QPixmap(this->size())),
      m_points_info(points_info),
      m_grid_step(step)
{
    this->setMouseTracking(true);
}

Viewer::~Viewer()
{
    delete m_pixmap;
}

void Viewer::resetPoints(const PointsInfo& points_info)
{
    m_points_info = &points_info;
    repaint();
}

const QPixmap* Viewer::getPixmap()
{
    return m_pixmap;
}

void Viewer::formPixmap()
{
    m_pixmap->fill(Qt::white);
    if (m_points_info->getStoredCount() < 2)
    {
        return;
    }

    // *** Choosing of scale for drawing *** //

    // Sizes of widget for drawing.
    const int w = this->width();
    const int h = this->height();
    const int min_size = qMin(w, h);
    // Indent from bounds of widget.
    const int dx = 35;
    // Sizes of zone for points drawing.
    const int w_work = w - 2 * dx;
    const int h_work = h - 2 * dx;
    // Values of points scatter along axis.
    const qreal x_scatter = m_points_info->getHDiff();
    const qreal y_scatter = m_points_info->getVDiff();
    // Scale of picture - count of pixels by unit of space.
    // First, try to scale with minimum side of screen.
    const bool is_height_max = min_size == h;
    const double pre_scale = is_height_max ? h_work / y_scatter :
                                             w_work / x_scatter;
    double scale = pre_scale;
    // If another side can't fit its data with chosen scale,
    // then scale picture with another side.
    if (min_size == h)
    {
        if (pre_scale * x_scatter > w_work)
        {
            scale = w_work / x_scatter;
        }
    }
    else
    {
        if (pre_scale * y_scatter > h_work)
        {
            scale = h_work / y_scatter;
        }
    }

    // *** Grid drawing. *** //
    QPainter painter;
    painter.begin(m_pixmap);
    painter.setPen(QPen(Qt::gray, 0.5));
    // Scatters of points for whole area of drawing.
    const qreal x_scatter_full = w / scale;
    const qreal y_scatter_full = h / scale;
    // Coordinates for grid drawing.
    const double dx_out_of_scale = dx / scale;
    double i = y_scatter_full - dx_out_of_scale;
    double j = dx_out_of_scale;
    while (i >= 0 )
    {
        painter.drawLine(0, i * scale, w, i * scale);
        i -= *m_grid_step;
    }
    i = y_scatter_full - dx_out_of_scale + *m_grid_step;
    while (i <= y_scatter_full)
    {
        painter.drawLine(0, i * scale, w, i * scale);
        i += *m_grid_step;
    }
    while (j < x_scatter_full)
    {
        painter.drawLine(j * scale, 0, j * scale, h);
        j += *m_grid_step;
    }
    j = dx_out_of_scale - *m_grid_step;
    while (j >= 0)
    {
        painter.drawLine(j * scale, 0, j * scale, h);
        j -= *m_grid_step;
    }

    // *** Points drawing. *** //
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, 2));
    painter.setFont(QFont("Times New Roman", 10));
    const QBrush brush_y(Qt::red);
    const QBrush brush_st = painter.brush();
    QFontMetrics metrics(painter.font());
    const int y_shift = 6 + metrics.height() / 3;

    const Points& points = m_points_info->getPoints();
    const qreal lb = m_points_info->getLeftBound();
    const qreal lwb = m_points_info->getLowerBound();
    for (Points::const_iterator it = points.begin();
         it != points.end(); ++it)
    {
        qreal x = dx + (it->x() - lb)  * scale;
        qreal y = dx + (it->y() - lwb) * scale;
        QPointF point(x, h - y);

        painter.drawEllipse(point, 6, 6);
        painter.setBrush(brush_y);
        painter.drawEllipse(point, 3, 3);
        painter.setBrush(brush_st);
        QString coord_text = "[" + QString::number(it->x()) + ", "
                                 + QString::number(it->y()) + "]";
        painter.drawText(x - metrics.width(coord_text) / 2,
                         point.y() - y_shift,
                         coord_text);
    }

    // *** Bounds values drawing. *** //
    const int ub_coord = y_scatter * scale;
    const int rb_coord = x_scatter * scale;

    QString str_minX = "Min X = " + QString::number(lb);
    QString str_maxX = "Max X = " + QString::number(m_points_info->getRightBound());
    QString str_minY = "Min Y = " + QString::number(lwb);
    QString str_maxY = "Max Y = " + QString::number(m_points_info->getUpperBound());

    painter.setPen(QPen(Qt::darkRed, 2));
    painter.setFont(QFont("Times New Roman", 12));
    QFontMetrics metrics2(painter.font());
    painter.drawText(dx,                                       h - 3, str_minX);
    painter.drawText(dx + rb_coord - metrics2.width(str_maxX), h - 3, str_maxX);
    painter.rotate(90);
    painter.drawText(h - (metrics2.width(str_minY) + dx),      -3,    str_minY);
    painter.drawText(h - (dx + ub_coord),                      -3,    str_maxY);
    painter.rotate(-90);

    // *** Axis drawing. *** //
    painter.setPen(QPen(Qt::darkRed, 2));
    painter.drawLine(1, 0,     1,     h - 1);
    painter.drawLine(0, h - 1, w - 1, h - 1);

    painter.end();
}

void Viewer::paintEvent(QPaintEvent *pEvent)
{
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

void Viewer::enterEvent(QEvent* event)
{
    if (event->type() == QEvent::Enter)
    {
        event->accept();
        QApplication::setOverrideCursor(Qt::PointingHandCursor);
     //   qDebug() << pEvent->x() << pEvent->y();
    }
    QWidget::enterEvent(event);
}

void Viewer::leaveEvent(QEvent* event)
{
    if (event->type() == QEvent::Leave)
    {
        event->accept();
   //     qDebug() << "Leave";
        QApplication::restoreOverrideCursor();
    }
    QWidget::leaveEvent(event);
}
