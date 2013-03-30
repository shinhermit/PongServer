#ifndef _Random
#define _Random

#include <QtGlobal>
#include <QTime>

namespace MathJ
{

class Random
{
public:
    Random(const uint & seed);
    Random();

    int randomInt();
    int randomIntbeetween(const int &first, const int &second);

private:
    static bool _seeded;
};

}

#endif // _Random
