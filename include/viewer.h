#ifndef VIEWER_H
#define VIEWER_H

#include <QWidget>
#include <QPixmap>
#include <QPaintEvent>
#include <QResizeEvent>

typedef QList<QPointF> Points;

struct PointsInfo
{
    PointsInfo() {}

    PointsInfo(const Points& p)
    {
        m_points = p;
        for (Points::const_iterator it = m_points.begin();
             it != m_points.end(); ++it)
        {
            if (it->x() < m_left)
            {
                m_left = it->x();
            }
            if (it->x() > m_right)
            {
                m_right = it->x();
            }
            if (it->y() < m_lower)
            {
                m_lower = it->y();
            }
            if (it->y() > m_upper)
            {
                m_upper = it->y();
            }
        }
        m_hdiff = m_right - m_left;
        m_vdiff = m_upper - m_lower;
    }

    void addPoint(const QPointF& point)
    {
        m_points.push_back(point);
        if (point.x() < m_left)
        {
            m_left = point.x();
        }
        if (point.x() > m_right)
        {
            m_right = point.x();
        }
        if (point.y() < m_lower)
        {
            m_lower = point.y();
        }
        if (point.y() > m_upper)
        {
            m_upper = point.y();
        }
        m_hdiff = m_right - m_left;
        m_vdiff = m_upper - m_lower;
    }

    void addPoint(qreal x, qreal y)
    {
        this->addPoint(QPointF(x,y));
    }

    void clear()
    {
       m_points.clear();
       m_left  =  std::numeric_limits<qreal>::max();
       m_right = -std::numeric_limits<qreal>::max();
       m_lower =  std::numeric_limits<qreal>::max();
       m_upper = -std::numeric_limits<qreal>::max();
       m_hdiff =  std::numeric_limits<qreal>::max();
       m_vdiff =  std::numeric_limits<qreal>::max();
    }

    qreal inline getUpperBound() const
    {
        return m_upper;
    }

    qreal inline getLowerBound() const
    {
        return m_lower;
    }

    qreal inline getLeftBound() const
    {
        return m_left;
    }

    qreal inline getRightBound() const
    {
        return m_right;
    }

    qreal inline getHDiff() const
    {
        return m_hdiff;
    }

    qreal inline getVDiff() const
    {
        return m_vdiff;
    }

    int inline getStoredCount() const
    {
        return m_points.size();
    }

    const inline Points& getPoints() const
    {
        return m_points;
    }

//private:
    Points m_points;
    qreal m_left  =  std::numeric_limits<qreal>::max();
    qreal m_right = -std::numeric_limits<qreal>::max();
    qreal m_lower =  std::numeric_limits<qreal>::max();
    qreal m_upper = -std::numeric_limits<qreal>::max();
    qreal m_hdiff =  std::numeric_limits<qreal>::max();
    qreal m_vdiff =  std::numeric_limits<qreal>::max();
};

class Viewer : public QWidget
{
    Q_OBJECT

public:
    explicit Viewer(const PointsInfo *points_info, const double* step, QWidget *parent = nullptr);
    ~Viewer();

    void resetPoints(const PointsInfo &points_info);

signals:

protected:
    void paintEvent(QPaintEvent *pEvent);
    void resizeEvent(QResizeEvent *pEvent);

private:
    void inline resetPixmap(QPixmap* p);
    void formPixmap();

private:
    QPixmap* m_pixmap = nullptr;
    const PointsInfo* m_points_info = nullptr;
    const double* m_grid_step = nullptr;
};

#endif // VIEWER_H
