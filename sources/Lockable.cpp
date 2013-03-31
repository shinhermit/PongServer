#include <Lockable.hpp>

void Lockable::lock()
{
    _locker.lock();
}

void Lockable::unlock()
{
    _locker.unlock();
}




