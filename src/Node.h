#ifndef NODE_H
#define NODE_H

#include <cmath>
#include <string>
#include <vector>

#include "Const.h"

struct Position
{
    double x, y;

    Position(double x, double y) : x(x), y(y) {}

    double dist2(const Position* loc) const {
        return (x - loc->x) * (x - loc->x) + (y - loc->y) * (y - loc->y);
    }

    double ED(const Position* loc) const
    {
        double theta =
            cos(y * Const::RAD) * cos(loc->y * Const::RAD) * cos((x - loc->x) * Const::RAD) +
            sin(y * Const::RAD) * sin(loc->y * Const::RAD);
        return Const::EARTH_RADIUS * acos(theta);
    }


    std::string toString() const;
};

struct Node : public Position
{
    Node(int id, double x, double y) : Position(x, y), id(id) {}

    int id;
};

#endif // NODE_H
