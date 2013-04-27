#ifndef _MathUtils
#define _MathUtils

#include <cmath>

/*! \namespace MathJ
 *namespace for all my classes that perform mathematical processings
 */
namespace MathJ
{

/*!
 * \brief The Trigo class performs trigonometric processings
 */
class Trigo
{
public:
    /*!
     * \brief Converts an angle in dregree to an angle in radian
     * \param alpha degree value of the angle
     * \return radian value of the angle
     */
    static double DegreeToRadian(const double & alpha);

    /*!
     * \brief Converts an angle in radian to an angle in degree
     * \param alpha radian value of the angle
     * \return degree value of the angle
     */
    static double RadianToDegree(const double & alpha);

    /*!
     * \brief Gets the value of Pi
     * \return Pi
     */
    static double Pi();
};

};
#endif // MATHUTILS_H
