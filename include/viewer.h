#ifndef VIEWER_H
#define VIEWER_H

#include "points.h"

#include <QWidget>
#include <QPixmap>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QMenu>

class Viewer : public QWidget
{
    Q_OBJECT

public:
    explicit Viewer(QWidget *parent = nullptr);
    ~Viewer();

    void resetPoints(const Points &points_info);
//    void addPoint(const QPointF& point);
    void clearPoints();
    const QPixmap* getPixmap();
    const Points& getPoints();
    void resetGridStep(double step);
    double getGridStep();

signals:
    void graphUpdated();
    void pointInserted(const QPointF&);
    void mouseEnterSignal();
    void mouseLeaveSignal();
    void mouseMoveSignal(double, double);

protected:
    void paintEvent(QPaintEvent *pEvent);
    void resizeEvent(QResizeEvent *pEvent);
    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);
    void contextMenuEvent(QContextMenuEvent* event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

protected slots:
    void insertPoint();
    void updateScale();

private:
    inline void resetPixmap(QPixmap* p)
    {
        delete m_pixmap;
        m_pixmap = p;
    }

    void formPixmap();

private:
    QPixmap*    m_pixmap = nullptr;
    QMenu*      m_context_menu = nullptr;

    PointsInfo  m_points_info;
    double      m_grid_step = -1;

    QPoint  m_insert_pos;
    int     m_indent = 35;
    double  m_scale = -1;
    // Measure of real space on one point of screen.
    // This value defines accuracy of point specifing.
    int     m_precision = -1;
};

#endif // VIEWER_H
