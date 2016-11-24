#ifndef SERVICE_H
#define SERVICE_H

#include "points.h"
#include "QTextStream"

bool readPoints(QTextStream& stream, Points& points);

QString convertToString(const Points& points);

QString loadInfo(const QString &filepath);

// Makes right precision coord of real coord by scale applying.
double getCoordWithPrecision(double coord, double scale);

#endif // SERVICE_H
