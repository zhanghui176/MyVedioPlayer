#ifndef UTILITY_H
#define UTILITY_H

#include <QString>
#include <QTime>

class Utility
{
public:
    Utility();

    static QString formatSecondToQTime(double seconds);
};

#endif // UTILITY_H
