#include "Trigo.hpp"

using namespace MathJ;

double Trigo::DegreeToRadian(const double & alpha)
{
    return (alpha * Pi()) / 180;
}


double Trigo::RadianToDegree(const double & alpha)
{
    return (alpha * 180) / Pi();
}

double Trigo::Pi()
{
    return ::acos(-1);
}
