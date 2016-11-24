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
    explicit Viewer(PointsInfo *points_info, const double* step, QWidget *parent = nullptr);
    ~Viewer();

    void resetPoints(PointsInfo &points_info);
    const QPixmap* getPixmap();

signals:
    void onPointInserted(const QPointF&);

protected:
    void paintEvent(QPaintEvent *pEvent);
    void resizeEvent(QResizeEvent *pEvent);
    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);
    void contextMenuEvent(QContextMenuEvent* event);
    void mousePressEvent(QMouseEvent *event);

protected slots:
    void insertPoint();

private:
    inline void resetPixmap(QPixmap* p)
    {
        delete m_pixmap;
        m_pixmap = p;
    }

    void formPixmap();

private:
    QPixmap*            m_pixmap = nullptr;
    QMenu*              m_context_menu = nullptr;
    PointsInfo*         m_points_info = nullptr;
    const double*       m_grid_step = nullptr;

    QPoint  m_insert_pos;
    int     m_indent = 35;
    double  m_scale = -1;
};

#endif // VIEWER_H
