#ifndef _Lockable
#define _Lockable

#include <QMutex>

class Lockable
{
public:
    static void lock();
    static bool tryLock();
    static void unlock();

    static void lock2(Lockable & locker1, Lockable & locker2);
    static void lock3(Lockable & locker1, Lockable & locker2, Lockable & locker3);

    static void unlock2(Lockable & locker1, Lockable & locker2);
    static void unlock3(Lockable & locker1, Lockable & locker2, Lockable & locker3);

private:
    static QMutex _locker;
};

#endif // LOCKABLE_HPP
