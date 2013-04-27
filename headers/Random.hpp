#ifndef _Random
#define _Random

#include <QtGlobal>
#include <QTime>

/*! \namespace MathJ
 *Namespace for all my classes that perform mathematical processings.
 */
namespace MathJ
{

/*!
 * \brief The Random class provides random integer generation
 */
class Random
{
public:
    /*!
     * \brief Constructor
     * \param seed has the same semantics as rand() seed
     */
    Random(const uint & seed);

    /*!
     * \brief Default constructor
     */
    Random();

    /*!
     * \brief generates a random integer
     * \return randomly generated integer
     */
    int randomInt();

    /*!
     * \brief generates an integer value in the range first-second
     * \param first min value for the randomly generated integer
     * \param second max value for the randomly generated integer
     * \return randomly generated integer
     */
    int randomIntbeetween(const int &first, const int &second);

private:
    static bool _seeded; /*!< avoids to seed again if already seeded*/
};

}

#endif // _Random
