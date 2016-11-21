#ifndef SERVICE_H
#define SERVICE_H

#include "include/points.h"
#include "QTextStream"

bool readPoints(QTextStream& stream, Points& points);

QString convertToString(const Points& points);

QString loadInfo(const QString &filepath);

#endif // SERVICE_H
