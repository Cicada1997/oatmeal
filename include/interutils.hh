#ifndef INTERUTILS_H
#define INTERUTILS_H

#include <cmath>

struct Vector { double x; double y; };
double dabs(double x) { return x >= 0 ? x : x*-1; }; // @brief calculates absolute value for doubles.
double vecDist(Vector pos1, Vector pos2)
{
    double dx2 = dabs(pos1.x - pos2.x) * dabs(pos1.x - pos2.x);
    double dy2 = dabs(pos1.y - pos2.y) * dabs(pos1.y - pos2.y);
    return sqrt(dx2 + dy2);
};

#endif
