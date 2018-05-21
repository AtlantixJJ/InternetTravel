#ifndef CAR_H
#define CAR_H

#include <cstring>
#include "Node.h"
#include "Solution.h"

class Map;

class Vehicle
{
public:
    Vehicle(int id, const Node* pos, const NodeList& passengers);
    virtual ~Vehicle();

    int getId() const { return m_id; }

    const Node* getPos() const { return m_pos; }

    const NodeList* getPassenger() const { return &m_passengers; }

    void print() const;

    Solution query(const Node* src, const Node* dst, const Map* map) const;

private:
    int m_id, m_passenger_count;
    const Node* m_pos;
    NodeList m_passengers;

    static double getMinDistance(std::vector<int>& dstIds, int srcId,
                                 const double disMatrix[6][6]);

    bool earthDistanceCheck(const Node* src, const Node* dst,
                            const Map* map) const;
};

typedef std::vector<const Vehicle*> VehicleList;

#endif // CAR_H
