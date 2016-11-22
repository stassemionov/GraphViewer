#ifndef VIEWER_H
#define VIEWER_H

#include "points.h"

#include <QWidget>
#include <QPixmap>
#include <QPaintEvent>
#include <QResizeEvent>

class Viewer : public QWidget
{
    Q_OBJECT

public:
    explicit Viewer(const PointsInfo *points_info, const double* step, QWidget *parent = nullptr);
    ~Viewer();

    void resetPoints(const PointsInfo &points_info);
    const QPixmap* getPixmap();

signals:

protected:
    void paintEvent(QPaintEvent *pEvent);
    void resizeEvent(QResizeEvent *pEvent);
    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);

private:
    inline void resetPixmap(QPixmap* p)
    {
        delete m_pixmap;
        m_pixmap = p;
    }

    void formPixmap();

private:
    QPixmap* m_pixmap = nullptr;
    const PointsInfo* m_points_info = nullptr;
    const double* m_grid_step = nullptr;
};

#endif // VIEWER_H
