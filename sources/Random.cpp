#include "Random.hpp"

bool MathJ::Random::_seeded = false;

MathJ::Random::Random(const uint & seed)
{
    if(!_seeded)
    {
        _seeded = true;

        qsrand(seed);
    }
}

MathJ::Random::Random()
{
    if(!_seeded)
    {
        _seeded = true;

        QTime time = QTime::currentTime();
        qsrand( (uint)time.msec() );
    }
}

int MathJ::Random::randomInt()
{
    return qrand();
}

int MathJ::Random::randomIntbeetween(const int & first, const int & second)
{
    return qrand() % ((second+ 1) - first) + first;
}

