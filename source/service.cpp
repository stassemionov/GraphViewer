#include "service.h"

#include <QDebug>
#include <QFile>

bool readPoints(QTextStream &stream, Points& points)
{
    qint64 pos = stream.pos();

    Points buffer;
    while (!stream.atEnd())
    {
        QString str1, str2;
        while (str1.isEmpty() && !stream.atEnd())
        {
            stream >> str1;
        }
        while (str2.isEmpty() && !stream.atEnd())
        {
            stream >> str2;
        }

        // Incorrectness: incomplete pair of number was specified.
        if (!str1.isEmpty() && str2.isEmpty())
        {
            stream.seek(pos);
            return false;
        }
        if (!str2.isEmpty())
        {
            bool success = false;
            double x = str1.toDouble(&success);
            if (!success)
            {
                stream.seek(pos);
                return false;
            }
            double y = str2.toDouble(&success);
            if (!success)
            {
                stream.seek(pos);
                return false;
            }
            buffer.push_back(QPointF(x, y));
        }
    }
    points.append(buffer);
    return true;
}

QString convertToString(const Points& points)
{
    QString result;
    QTextStream stream(&result, QIODevice::WriteOnly);
    for (Points::const_iterator it = points.begin();
         it != points.end(); ++it)
    {
        stream << QString::number(it->x()) << " " << QString::number(it->y()) << "\n";
    }
    return result;
}

QString loadInfo(const QString& filepath)
{
    QFile file(filepath);
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QString result = stream.readAll();
    file.close();

    return result;
}

int getPrecision(double value)
{
    if (value < 1)
    {
        double ten_in_power = 1;
        while (value <= 1)
        {
            value *= 10;
            ten_in_power *= 10;
        }
        return ten_in_power;
    }
    else
    {
        double ten_in_power = 0.1;
        while (value >= 1)
        {
            value /= 10;
            ten_in_power *= 10;
        }
        return -ten_in_power;
    }
}

double getValueWithPrecision(double value, int precision)
{
    if (precision > 0)
    {
        return floor(value * precision) / precision;
    }
    else
    {
        precision *= -1;
        return floor(value / precision) * precision;
    }
}
