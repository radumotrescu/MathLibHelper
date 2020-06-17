#include <numeric>

#include <VecN.h>

bool EQ(double a, double b, double epsilon = std::numeric_limits<double>::epsilon())
{
    return std::fabs(a - b) < epsilon;
}

using uint = unsigned int;
