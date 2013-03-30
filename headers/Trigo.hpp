#ifndef _MathUtils
#define _MathUtils

#include <cmath>

namespace MathJ
{

class Trigo
{
public:
    static double DegreeToRadian(const double & alpha);

    static double RadianToDegree(const double & alpha);

    static double Pi();
};

};
#endif // MATHUTILS_H
