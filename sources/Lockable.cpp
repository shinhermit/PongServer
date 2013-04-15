#include <Lockable.hpp>

void Lockable::lock()
{
    _locker.lock();
}

bool Lockable::tryLock()
{
    return _locker.tryLock();
}

void Lockable::unlock()
{
    _locker.unlock();
}

void Lockable::lock2(Lockable &locker1, Lockable &locker2)
{
    bool locked;

    locked = false;
    while(!locked)
    {
        locked = locker1.tryLock();
        if(locked)
        {
            locked = locker2.tryLock();
            if(!locked)
                locker1.unlock();
        }
    }
}

void Lockable::lock3(Lockable &locker1, Lockable &locker2, Lockable &locker3)
{
    bool locked;

    locked = false;
    while(!locked)
    {
        locked = locker1.tryLock();
        if( locked )
        {
            locked = locker2.tryLock();
            if(locked)
            {
                locked = locker3.tryLock();
                if( !locked )
                {
                    locker1.unlock();
                    locker2.unlock();
                }
            }

            else
                locker3.unlock();
        }
    }
}

void Lockable::unlock2(Lockable &locker1, Lockable &locker2)
{
    locker1.unlock();
    locker2.unlock();
}


void Lockable::unlock3(Lockable &locker1, Lockable &locker2, Lockable &locker3)
{
    locker1.unlock();
    locker2.unlock();
    locker3.unlock();
}



