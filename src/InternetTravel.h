#ifndef INTERNET_TRAFFIC_H
#define INTERNET_TRAFFIC_H

#include <string>

#include "Car.h"
#include "Map.h"
#include "Solution.h"

class InternetTravel
{
public:
    InternetTravel();
    virtual ~InternetTravel();

    const Map* getMap() const { return m_map; }

    void startup(const std::string& dataDir);

    // Query by (x, y)
    // SolutionList query(double st_x, double st_y, double ed_x, double ed_y);
    // Query by node
    SolutionList query(const Node* src, const Node* dst);

private:
private:
    Map* m_map;
    CarList m_cars;

    void loadCars(const std::string& dataFile);
};

#endif // INTERNET_TRAFFIC_H
