#ifndef CAR_H
#define CAR_H

#include <cstring>
#include "Node.h"
#include "Solution.h"

class Map;

class Vehicle
{
public:
    Vehicle(int id, const Node* pos, const std::vector<const Node*>& passengers);
    virtual ~Vehicle();

    int getId() const { return m_id; }

    const Node* getPos() const { return m_pos; }

    const std::vector<const Node*>* getPassenger() const { return &m_passengers; }

    void print() const;

    Solution query(const Node* src, const Node* dst, const Map* map) const;

    /// Given the source, enumerate the arriving order, and store in order
    static double getMinDist(std::vector<int>& order, int srcId,
                                 const double all_dist[6][6]);

private:
    /// simple filter
    bool filter(const Node* src, const Node* dst,
                            const Map* map) const;

private:
    int m_id, m_passenger_count;
    const Node* m_pos;
    std::vector<const Node*> m_passengers;
};

#endif // CAR_H
