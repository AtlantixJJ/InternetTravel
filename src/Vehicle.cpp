#include "Vehicle.h"
#include "Map.h"

using namespace std;

/// Directly store all the permutations of n <= 5
const int FACTORIAL[5] = {1, 1, 2, 6, 24};
const int PERMUTATIONS[5][24][4] = {
    {},
    {{0}},
    {{0, 1}, {1, 0}},
    {{0, 1, 2}, {0, 2, 1}, {1, 0, 2}, {1, 2, 0}, {2, 0, 1}, {2, 1, 0}},
    {
        {0, 1, 2, 3}, {0, 1, 3, 2}, {0, 2, 1, 3}, {0, 2, 3, 1}, {0, 3, 1, 2},
        {0, 3, 2, 1}, {1, 0, 2, 3}, {1, 0, 3, 2}, {1, 2, 0, 3}, {1, 2, 3, 0},
        {1, 3, 0, 2}, {1, 3, 2, 0}, {2, 0, 1, 3}, {2, 0, 3, 1}, {2, 1, 0, 3},
        {2, 1, 3, 0}, {2, 3, 0, 1}, {2, 3, 1, 0}, {3, 0, 1, 2}, {3, 0, 2, 1},
        {3, 1, 0, 2}, {3, 1, 2, 0}, {3, 2, 0, 1}, {3, 2, 1, 0},
    },
};

Vehicle::Vehicle(int id, const Node* pos, const std::vector<const Node*>& passengers)
    : m_id(id), m_passenger_count(passengers.size()), m_pos(pos),
      m_passengers(passengers)
{
}

Vehicle::~Vehicle() {}

void Vehicle::print() const
{
    printf("car #%d: %d %s\n", m_id, m_passenger_count,
           m_pos->toString().c_str());
    printf("\t[ ");
    for (auto node : m_passengers)
        printf("%s, ", node->toString().c_str());
    printf("]\n");
}

double Vehicle::getMinDist(vector<int>& order, int srcId,
                           const double all_dist[6][6])
{
    int prev, n = order.size();
    double res = Const::INF;
    vector<int> res_order = order;

    if (n == 0) return 0;

    for (int i = 0; i < FACTORIAL[n]; i++) {
        vector<int> temp;
        double d = 0;
        prev = srcId;
        for (int j = 0; j < n; j++) {
            int cur = order[PERMUTATIONS[n][i][j]];
            d += all_dist[prev][cur];
            if (d >= res) break; // early exit
            temp.push_back(cur);
            prev = cur;
        }
        if (d < res) {
            res = d;
            res_order = temp;
        }
    }

    order = res_order;
    return res;
}

bool Vehicle::filter(const Node* src, const Node* dst,
                             const Map* map) const
{
    /// Passenger limit
    if (m_passenger_count >= 4)
        return false;

    /// If the pick-up distance is larger than 10 km, then discard
    double d2 = map->distance(m_pos, src);
    return d2 <= 10;
}

Solution Vehicle::query(const Node* src, const Node* dst, const Map* map) const
{
    printf("Car query.\n");
    /// filter not passed, return an empty solution
    if (!filter(src, dst, map))
        return Solution();

    /// Get distance matrix for all points    
    std::vector<const Node*> nodes = {m_pos, src, dst};
    for (auto node : m_passengers)
        nodes.push_back(node);

    double all_dist[6][6];
    memset(all_dist, 0, sizeof(all_dist));
    for (int i = 0; i < nodes.size(); i++)
        for (int j = 0; j < i; j++)
        {
            all_dist[i][j] = all_dist[j][i] =
                map->roadmap_distance(nodes[i], nodes[j]);
            if (all_dist[0][1] > 10)
                return Solution();
        }

    /// d2 : car->src distance
    double d2 = all_dist[0][1];
    /// d4 : src->dst distance
    double d4 = all_dist[1][2];

    /// enumerate passenger order
    vector<int> order;
    for (int i = 0; i < m_passengers.size(); i++)
        order.push_back(i + 3); // car src dst is 0, 1, 2

    /// original distance: car & all pass
    double d1 = getMinDist(order, 0, all_dist);

    /// dist after pick up: src & all pass & dst
    order.push_back(2);
    double d3 = getMinDist(order, 1, all_dist);

    /// Total dist
    order.push_back(1);
    double d5 = getMinDist(order, 0, all_dist);

    double detour_dis1 = d5 - d1;
    double detour_dis2 = d3 - d4;

    if (detour_dis1 > 10 || detour_dis2 > 10) {
        printf("No solution.\n");
        return Solution();
    }

    std::vector<const Node*> pass_order = {m_pos};
    printf("Permute done.\n");
    for (auto i : order)
    {
        if (i == 1)
            pass_order.push_back(src);
        else if (i == 2)
            pass_order.push_back(dst);
        else if (i >= 3)
            pass_order.push_back(m_passengers[i - 3]);
    }
    printf("Vehicle done.\n");
    return Solution(this, pass_order, d2, detour_dis1, detour_dis2);
}
