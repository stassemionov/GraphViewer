#include "points.h"

PointsInfo::PointsInfo()
{
    this->clear();
}

PointsInfo::PointsInfo(const Points& p)
{
    this->construct(p);
}

void PointsInfo::addPoint(const QPointF& point)
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

void PointsInfo::clear()
{
   m_points.clear();
   m_left  =  std::numeric_limits<qreal>::max();
   m_right = -std::numeric_limits<qreal>::max();
   m_lower =  std::numeric_limits<qreal>::max();
   m_upper = -std::numeric_limits<qreal>::max();
   m_hdiff =  std::numeric_limits<qreal>::max();
   m_vdiff =  std::numeric_limits<qreal>::max();
}

void PointsInfo::construct(const Points &points_init)
{
    this->clear();

    m_points = points_init;
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
