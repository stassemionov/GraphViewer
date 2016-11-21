#ifndef POINTS_H
#define POINTS_H

#include <QList>
#include <QPointF>

typedef QList<QPointF> Points;

struct PointsInfo
{
    PointsInfo();
    PointsInfo(const Points& p);

    void clear();
    void addPoint(const QPointF& point);

    inline void addPoint(qreal x, qreal y)
    {
        this->addPoint(QPointF(x,y));
    }

    inline qreal getUpperBound() const
    {
        return m_upper;
    }

    inline qreal getLowerBound() const
    {
        return m_lower;
    }

    inline qreal getLeftBound() const
    {
        return m_left;
    }

    inline qreal getRightBound() const
    {
        return m_right;
    }

    inline qreal getHDiff() const
    {
        return m_hdiff;
    }

    inline qreal getVDiff() const
    {
        return m_vdiff;
    }

    inline int getStoredCount() const
    {
        return m_points.size();
    }

    inline const Points& getPoints() const
    {
        return m_points;
    }

    inline void setPoints(const Points &points_init)
    {
        this->construct(points_init);
    }

private:
    void construct(const Points &points_init);

private:
    Points m_points;
    qreal m_left  =  std::numeric_limits<qreal>::max();
    qreal m_right = -std::numeric_limits<qreal>::max();
    qreal m_lower =  std::numeric_limits<qreal>::max();
    qreal m_upper = -std::numeric_limits<qreal>::max();
    qreal m_hdiff =  std::numeric_limits<qreal>::max();
    qreal m_vdiff =  std::numeric_limits<qreal>::max();
};


#endif // POINTS_H
