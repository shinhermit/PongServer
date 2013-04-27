#ifndef CONCURRENT_HPP
#define CONCURRENT_HPP

#include <QString>
#include <QDebug>

#include "PongShared.hpp"

/*!
 * \brief The Concurrent class
 * This class regroups the common services needed in all concurrent classes in the program, which, globally speaking, are classes that need to access the static objects of the PongShared class.
 */
class Concurrent : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Constructor
     * \param parent the parent QObject, passed to the constructor the QObject superclass
     */
    Concurrent(QObject * parent=0);

    /*!
     * \brief deadlock-safe lock of PongShared::GameState
     * This method avoids to fall into a deadlock because of calling Mutex.lock after having already got the mutex lock. This situation can easily emerge when you lock a mutex, and then call a method that locks the mutex internally. This situation is not easy to detect while coding.
     */
    void lockGameState();

    /*!
     * \brief deadlock-safe lock of PongShared::PlayingArea
     * This method avoids to fall into a deadlock because of calling Mutex.lock after having already got the mutex lock. This situation can easily emerge when you lock a mutex, and then call a method that locks the mutex internally. This situation is not easy to detect while coding.
     */
    void lockPlayingArea();

    /*!
     * \brief deadlock-safe lock of PongShared::PlayersStates.
     * This method avoids to fall into a deadlock because of calling Mutex.lock after having already got the mutex lock. This situation can easily emerge when you lock a mutex, and then call a method that locks the mutex internally. This situation is not easy to detect while coding.
     */
    void lockPlayersStates();

    /*!
     * \brief dual of lockGameState
     */
    void unlockGameState();

    /*!
     * \brief dual of lockPlayingArea
     */
    void unlockPlayingArea();

    /*!
     * \brief dual of lockPlayersStates
     */
    void unlockPlayersStates();

public slots:
    /*!
     * \brief intended to terminate the thread which executes this worker
     * We call Concurrent classes, classes which need to access the pbjects in the class PongShared.
     *Inside the methods of all Concurrent classes, there are break points, that allow to stop the execution safely, and then send the signal finishedSignal(), which should be connected to the slot quit() of the thread which executes this worker class.
     */
    void notBusyQuit();

protected:
    /*!
     * \brief checks if an exit state have been set.
     * \return true if PongShared::gameState.state() is PongTypes::EXIT_REQUESTED, false elsewise.
     */
    bool _exit_requested();

    /*!
     * \brief emits (only one time) finishedSignal
     *finishedSignal is intended to stop the thread that executes this worker.
     */
    void _finish();

signals:
    /*!
     * \brief intended to be connected to the slot quit() of the thread that executes the worker class.
     */
    void finishedSignal();

private:
    bool _I_locked_gameState; /*!< flag that tell is this worker already locked PongShared::gameState*/
    bool _I_locked_playingArea; /*!< flag that tell is this worker already locked PongShared::playingArea*/
    bool _I_locked_playersStates; /*!< flag that tell is this worker already locked PongShared::playersStates*/
    bool _finishEmitted; /*!< flag that tell is this worker already emitted finishedSignal*/
};

#endif // CONCURRENT_HPP
