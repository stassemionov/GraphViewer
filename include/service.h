#ifndef SERVICE_H
#define SERVICE_H

#include "points.h"
#include "QTextStream"

bool readPoints(QTextStream& stream, Points& points);

QString convertToString(const Points& points);

QString loadInfo(const QString &filepath);

int getPrecision(double value);

// Makes value with specified precision.
double getValueWithPrecision(double value, int precision);

#endif // SERVICE_H
