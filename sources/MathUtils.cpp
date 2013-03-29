#include "MathUtils.hpp"

double MathUtils::DegreeToRadian(const double & alpha)
{
    return (alpha * Pi()) / 180;
}


double MathUtils::RadianToDegree(const double & alpha)
{
    return (alpha * 180) / Pi();
}

double MathUtils::Pi()
{
    return ::acos(-1);
}
