#ifndef NODE_H
#define NODE_H

#include <cmath>
#include <string>
#include <vector>

#include "Const.h"

struct Location
{
    Location(double x, double y) : x(x), y(y) {}

    double x, y;

    double distanceTo2(const Location* loc) const
    {
        return (x - loc->x) * (x - loc->x) + (y - loc->y) * (y - loc->y);
    }

    double distanceTo(const Location* loc) const
    {
        return sqrt(distanceTo2(loc));
    }

    double ED(const Location* loc) const
    {
        constexpr double rad = Const::PI / 180;
        double theta =
            cos(y * rad) * cos(loc->y * rad) * cos((x - loc->x) * rad) +
            sin(y * rad) * sin(loc->y * rad);
        return Const::EARTH_RADIUS * acos(theta);
    }

    std::string toString() const;
};

struct Node : public Location
{
    Node(int id, double x, double y) : Location(x, y), id(id) {}

    int id;
};

#endif // NODE_H
