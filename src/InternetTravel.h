#ifndef INTERNET_TRAVEL_H
#define INTERNET_TRAVEL_H

#include <string>

#include "Vehicle.h"
#include "Map.h"
#include "Solution.h"

/**
 * Computation backend
 * Initialize by giving the data dir
 **/
class InternetTravel
{
public:
    InternetTravel();
    virtual ~InternetTravel();

    /// First call this to initialize
    void init(const std::string& dataDir);

    const Map* getMap() const { return m_map; }

    /// Query by node
    SolutionList query(const Node* src, const Node* dst);
    /// Query by (x, y)
    SolutionList query(double st_x, double st_y, double ed_x, double ed_y);

private:
    /// Load car file
    void loadCars(const std::string& dataFile);

private:
    Map* m_map;
    std::vector<const Vehicle*> m_cars;
};

#endif // INTERNET_travel_H
