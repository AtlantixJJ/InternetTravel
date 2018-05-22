#ifndef CONST_H
#define CONST_H

namespace Const
{
constexpr double INF = 1e99;
constexpr double PI = 3.14159265358979323846;
constexpr double RAD = PI / 180;
constexpr double EARTH_RADIUS = 6371.0;

/// Directly store all the permutations of n <= 5
extern int FACTORIAL[6];
extern bool fac_num[6];
extern int fac_cnt;
extern int PERMUTATIONS[6][121][5];

void write_permutations(int depth, int N);
} // namespace Const

#endif // CONST_H
