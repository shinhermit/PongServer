#ifndef _Lockable
#define _Lockable

#include <QMutex>

class Lockable
{
public:
    void lock();
    void unlock();

private:
    QMutex _locker;
};

#endif // LOCKABLE_HPP
