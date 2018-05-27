#ifndef SOLUTION_H
#define SOLUTION_H

#include "Const.h"
#include "Node.h"

class Vehicle;

/**
 * Solution storage
 **/
struct Solution
{
    Solution() : dis(Const::INF) {}
    Solution(const Vehicle* car, const std::vector<const Node*>& path, double dis,
             double d1, double d2)
        : car(car), path(path), dis(dis), detour_dis1(d1),
          detour_dis2(d2)
    {
        value = detour_dis1 + detour_dis2;
    }

    const Vehicle* car;
    std::vector<const Node*> path; /// 完整路径
    std::vector<int> order; /// 送达乘客的顺序

    double dis;         /// 接到当前乘客的距离
    double detour_dis1; /// 已经上车乘客的绕路距离
    double detour_dis2; /// 新乘客的绕路距离

    double value; // detour_dis1 + detour_dis2

    bool is_valid() const { return dis <= 10; }

    bool operator<(const Solution& sol) const {
        // 
        return value < sol.value || (value == sol.value && detour_dis1 < sol.detour_dis1);
    }
};

typedef std::vector<Solution> SolutionList;

#endif // SOLUTION_H
