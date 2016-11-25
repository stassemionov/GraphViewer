#include "viewer.h"
#include "service.h"

#include <QPainter>
#include <QApplication>
#include <QDebug>

Viewer::Viewer(QWidget *parent)
    : QWidget(parent)
{
    this->setMouseTracking(true);

    m_pixmap = new QPixmap(this->size());

    // Context menu creation.
    m_context_menu = new QMenu(this);
    QAction* add_point_action = new QAction(
                QString::fromLocal8Bit("Установить точку"),
                m_context_menu);
    m_context_menu->addAction(add_point_action);
    connect(add_point_action, SIGNAL(triggered()),
            this,             SLOT(insertPoint()));
}

Viewer::~Viewer()
{
    delete m_pixmap;
    delete m_context_menu;
}

void Viewer::resetPoints(const Points& points)
{
    if (points.empty())
    {
        return;
    }

    m_points_info.setPoints(points);
    // By default, step equals 1/20 from minimum side of points rectangle.
    m_grid_step = qMax(m_points_info.getHDiff(),
                       m_points_info.getVDiff()) / 20;

    this->repaint();
    emit graphUpdated();
}

void Viewer::insertPoint()
{
    // Screen coords of pointed place.
    const int x = m_insert_pos.x();
    const int y = this->height() - m_insert_pos.y() - 1;
    // Real coords of points, but with not optimal precision.
    double rx = m_points_info.getLeftBound() +
                (x - m_indent) / m_scale;
    double ry = m_points_info.getLowerBound() +
                (y - m_indent) / m_scale;
    // Addition of points with optimal precision.
    m_points_info.addPoint(getValueWithPrecision(rx, m_precision),
                           getValueWithPrecision(ry, m_precision));
    this->repaint();

    emit pointInserted(m_points_info.getPoints().back());
}

const QPixmap* Viewer::getPixmap()
{
    return m_pixmap;
}

//void Viewer::addPoint(const QPointF& point)
//{
//    m_points_info.addPoint(point);
//}

void Viewer::clearPoints()
{
    m_points_info.clear();
    m_grid_step = -1;
    m_scale = -1;

    this->repaint();
    emit graphUpdated();
}

const Points& Viewer::getPoints()
{
    return m_points_info.getPoints();
}

void Viewer::resetGridStep(double step)
{
    m_grid_step = step;

    this->repaint();
    emit graphUpdated();
}

double Viewer::getGridStep()
{
    return m_grid_step;
}

void Viewer::formPixmap()
{
    m_pixmap->fill(Qt::white);
    if (m_points_info.getStoredCount() < 2)
    {
        return;
    }

    // Sizes of widget for drawing.
    const int w = this->width();
    const int h = this->height();
    // Indent from bounds of widget.
    const int dx = m_indent;

    // *** Grid drawing. *** //
    QPainter painter;
    painter.begin(m_pixmap);
    painter.setPen(QPen(Qt::gray, 0.5));
    // Values of points scatter along axis.
    const qreal x_scatter = m_points_info.getHDiff();
    const qreal y_scatter = m_points_info.getVDiff();
    // Scatters of points for whole area of drawing.
    const qreal x_scatter_full = w / m_scale;
    const qreal y_scatter_full = h / m_scale;
    // Coordinates for grid drawing.
    const double dx_out_of_scale = dx / m_scale;
    double i = y_scatter_full - dx_out_of_scale;
    double j = dx_out_of_scale;
    while (i >= 0 )
    {
        painter.drawLine(0, i * m_scale, w - 1, i * m_scale);
        i -= m_grid_step;
    }
    i = y_scatter_full - dx_out_of_scale + m_grid_step;
    while (i <= y_scatter_full)
    {
        painter.drawLine(0, i * m_scale, w - 1, i * m_scale);
        i += m_grid_step;
    }
    while (j < x_scatter_full)
    {
        painter.drawLine(j * m_scale, 0, j * m_scale, h - 1);
        j += m_grid_step;
    }
    j = dx_out_of_scale - m_grid_step;
    while (j >= 0)
    {
        painter.drawLine(j * m_scale, 0, j * m_scale, h - 1);
        j -= m_grid_step;
    }

    // *** Points drawing. *** //
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, 2));
    painter.setFont(QFont("Times New Roman", 10));
    const QBrush brush_y(Qt::red);
    const QBrush brush_st = painter.brush();
    QFontMetrics metrics(painter.font());
    const int y_shift = 6 + metrics.height() / 3;

    const Points& points = m_points_info.getPoints();
    const qreal lb = m_points_info.getLeftBound();
    const qreal lwb = m_points_info.getLowerBound();
    for (Points::const_iterator it = points.begin();
         it != points.end(); ++it)
    {
        qreal x = dx + (it->x() - lb)  * m_scale;
        qreal y = dx + (it->y() - lwb) * m_scale;
        QPointF point(x, h - y - 1);

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
    const int ub_coord = y_scatter * m_scale;
    const int rb_coord = x_scatter * m_scale;

    QString str_minX = "Min X = " + QString::number(lb);
    QString str_maxX = "Max X = " + QString::number(m_points_info.getRightBound());
    QString str_minY = "Min Y = " + QString::number(lwb);
    QString str_maxY = "Max Y = " + QString::number(m_points_info.getUpperBound());

    painter.setPen(QPen(Qt::darkRed, 2));
    painter.setFont(QFont("Times New Roman", 12));
    QFontMetrics metrics2(painter.font());
    const int str_minX_w = metrics2.width(str_minX);
    const int str_maxX_w = metrics2.width(str_maxX);
    const int str_minY_w = metrics2.width(str_minY);
    const int str_maxY_w = metrics2.width(str_maxY);

    painter.drawText(dx, h - 3, str_minX);
    painter.drawText(dx + qMax(str_minX_w + 10, rb_coord - str_maxX_w),
                     h - 3, str_maxX);
    painter.rotate(90);
    painter.drawText(h - (str_minY_w + dx), -3, str_minY);
    painter.drawText(h - (dx + qMax(str_minY_w + str_maxY_w + 10, ub_coord)),
                     -3, str_maxY);
    painter.rotate(-90);

    // *** Axis drawing. *** //
    painter.setPen(QPen(Qt::darkRed, 2));
    painter.drawLine(1, 0,     1,     h - 1);
    painter.drawLine(0, h - 1, w - 1, h - 1);

    painter.end();
}

void Viewer::updateScale()
{
    if (m_points_info.getStoredCount() < 2)
    {
        return;
    }

    const int w = this->width();
    const int h = this->height();
    // Indent from bounds of widget.
    const int dx = m_indent;
    // Sizes of zone for points drawing.
    const int w_work = w - 2 * dx;
    const int h_work = h - 2 * dx;
    // Values of points scatter along axis.
    const qreal x_scatter = m_points_info.getHDiff();
    const qreal y_scatter = m_points_info.getVDiff();
    // Scale of picture - count of pixels by unit of space.
    // First, try to scale with minimum side of screen.
    const bool is_height_max = qMin(w, h) == h;
    const double pre_scale = is_height_max ? h_work / y_scatter :
                                             w_work / x_scatter;
    m_scale = pre_scale;
    // If another side can't fit its data with chosen scale,
    // then scale picture with another side.
    if (is_height_max)
    {
        if (pre_scale * x_scatter > w_work)
        {
            m_scale = w_work / x_scatter;
        }
    }
    else
    {
        if (pre_scale * y_scatter > h_work)
        {
            m_scale = h_work / y_scatter;
        }
    }
    m_precision = getPrecision(1.0 / m_scale);
}

void Viewer::paintEvent(QPaintEvent *pEvent)
{
    this->updateScale();
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
    event->accept();
    QApplication::setOverrideCursor(Qt::PointingHandCursor);
    emit mouseEnterSignal();
}

void Viewer::leaveEvent(QEvent* event)
{
    event->accept();
    QApplication::restoreOverrideCursor();
    emit mouseLeaveSignal();
}

void Viewer::contextMenuEvent(QContextMenuEvent* event)
{
    if (m_points_info.getStoredCount() >= 2)
    {
        event->accept();
        m_context_menu->exec(event->globalPos());
    }
}

void Viewer::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        event->accept();
        m_insert_pos = event->pos();
    }
}

void Viewer::mouseMoveEvent(QMouseEvent *event)
{
    if (m_points_info.getStoredCount() > 1)
    {
        event->accept();

        int x = event->pos().x();
        int y = this->height() - event->pos().y() - 1;
        // Real coords of points, but with not optimal precision.
        double rx = m_points_info.getLeftBound() +
                    (x - m_indent) / m_scale;
        double ry = m_points_info.getLowerBound() +
                    (y - m_indent) / m_scale;

        emit mouseMoveSignal(getValueWithPrecision(rx, m_precision),
                             getValueWithPrecision(ry, m_precision));
    }
}
