#ifndef _Lockable
#define _Lockable

#include <QMutex>

/*!
 * \brief The Lockable class is intended to ensure access to shared objects.
 *Shared objects are gathered in the class PongShared as static members. The main purpose of this class is to avoid the need of passing mutexes to every method that need to access the shared object. However the addition of the class Concurrent makes this class less usefull, we kept it for concenience.
 */
class Lockable
{
public:
    /*!
     * \brief locks the internal mutex
     */
    static void lock();

    /*!
     * \brief try to lock the internal mutex and return immediatly if fails
     * \return true if locked successfuly, false elsewise
     */
    static bool tryLock();

    /*!
     * \brief unlocks the internal mutex
     */
    static void unlock();

    /*!
     * \brief lock2 is intended to insure locking 2 objects
     * \param locker1 Lockable object 1
     * \param locker2 Lockable object 2
     */
    static void lock2(Lockable & locker1, Lockable & locker2);


    /*!
     * \brief lock3 is intended to insure locking 3 objects
     * \param locker1 Lockable object 1
     * \param locker2 Lockable object 2
     * \param locker3 Lockable object 3
     */
    static void lock3(Lockable & locker1, Lockable & locker2, Lockable & locker3);

    /*!
     * \brief dual of lock2
     * \param locker1 Lockable object 1
     * \param locker2 Lockable object 2
     */
    static void unlock2(Lockable & locker1, Lockable & locker2);

    /*!
     * \brief dual of lock3
     * \param locker1 Lockable object 1
     * \param locker2 Lockable object 2
     * \param locker3 Lockable object 3
     */
    static void unlock3(Lockable & locker1, Lockable & locker2, Lockable & locker3);

private:
    static QMutex _locker; /*!< internal mutex */
};

#endif // LOCKABLE_HPP
