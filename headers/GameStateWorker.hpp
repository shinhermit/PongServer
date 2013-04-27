#ifndef _GameStateWorker
#define _GameStateWorker

#include <cassert>

#include <QObject>
#include <QVector>
#include <QTimer>

#include "Concurrent.hpp"
#include "PongShared.hpp"
#include "PongTypes.hpp"
#include "PongServerView.hpp"

/*!
 * \brief The GameStateWorker manages the state of the game
 *The class is inteded to be executed in a thread and to access PongShared objects, thus inherits the class Concurrent.
 */
class GameStateWorker : public Concurrent
{
    Q_OBJECT
public:
    /*!
     * \brief constructor
     * \param parent passed to the constructor of Concurrent.
     */
    GameStateWorker(QObject * parent=0);

signals:
    /*!
     * \brief this signal is emitted when we to tell initialize the game
     *The signal should be internally connected to the slot checkInitSlot()
     */
    void checkInitSignal();

    /*!
     * \brief this signal is emitted when we to be start the playing game routine
     *This signal should be internally connected to the slot checkRunningSlot()
     */
    void checkRunningSignal();

    /*!
     * \brief this signal is use to append a status message in the PongServer view
     * \param status the status message we want to append
     */
    void appendStatusSignal(const QString & status);

    /*!
     * \brief this signal is emmited after a gameover state is encountered
     *This signal souhld be connected inside the PongServer to the PongServer::gameOverSlot();
     */
    void gameOverSignal();

    /*!
     * \brief tells BallMover to start updating the position of the ball
     *This signal should be connected inside PongServer to the BallMover::startMoving() slot
     */
    void startMovingBall();

    /*!
     * \brief tells BallMover to stop updating the position of the ball
     *This signal should be connected inside PongServer to the BallMover::stopMoving() slot
     */
    void stopMovingBall();

public slots:
    /*!
     * \brief this enters in a waitStart routine
     *The waitStart routine checks periodically the state of the game until the PongTypes::START_REQUESTED is encountered. It then emits checkInitSignal()
     *This slot should be connected inside PongServer to the signal startService() of PongServer.
     */
    void waitStartSlot();

    /*!
     * \brief This slot initializes the game.
     *Partucularly, it sets a count-down before the game is lunched, allowing the clients to get ready. After down-counting, it emits checkRunningSignal().
     *It should be internally connected to checkInitSignal(), which is emitted from waitStartSlot()
     */
    void checkInitSlot();

    /*!
     * \brief enters the game routine.
     *This slot periodically checks the global state of game, in order to detect:
     *. collision
     *. goal
     *. exit game request
     *. gameover state
     *. etc.
     *It should be internally connected to checkRunningSignal()
     */
    void checkRunningSlot();

    /*!
     * \brief reimplements Concurrent::notBusyQuit()
     *This slot reimplements Concurrent::notBusyQuit() because of the need to stop the internal timer.
     */
    void notBusyQuit();

private slots:
    /*!
     * \brief implements a count-down
     */
    void _countDownSlot();

    /*!
     * \brief the routine runned by waitStartSlot()
     */
    void _wait_start_routine();

    /*!
     * \brief the routine runned by checkRunningSlot()
     */
    void _check_running_routine();

private:
    QTimer _timer; /*!< the timer used by for the down-counting*/
    qint32 _downCounter; /*!< the down-counted value*/

    /*!
     * \brief updates rackets position
     *Updates the position of each racket according to those given by playersStates, which means collected by SocketWorker
     */
    void _update_rackets();

    /*!
     * \brief checks if the ball collides another element
     *Actually calls PongShared::playinArea.collicionHappened()
     */
    void _check_collisions();

    /*!
     * \brief takes action when the ball collides a cage
     * \param cageIndex index of the cage with which the ball collided
     */
    void _manage_goal(const int & cageIndex);

    /*!
     * \brief discards a players when his credit reaches zero
     * \param racketIndex index of the racket of the player. Generally, the index of the corresponding cage is passed instead.
     */
    void _discard_player(const int & racketIndex);

    /*!
     * \brief mirrors the ball after a collision with a wall
     * \param wallIndex indice of the wall that collided with the ball
     */
    void _manage_wall_collision(const int & wallIndex);

    /*!
     * \brief mirrors the ball after a collision with a racket.
     * \param racketIndex indice of the racket that collided with the ball.
     */
    void _manage_racket_collision(const int & racketIndex);

    /*!
     * \brief checks PongShared::gameState for a PongTypes::GAMEOVER state
     * \return true a gameover state is encounterd, false elsewise.
     */
    bool _game_over();

    /*!
     * \brief takes actions when a gameover state is encountered
     *The last action is to emit gameOverSignal(), intended to PongServer
     */
    void _manage_game_over();

    /*!
     * \brief tell if at least two players connected remain
     * \return true if at least two players have not a DISCONNECTED state, false elsewise.
     */
    bool _enough_players();

    /*!
     * \brief takes actions when too many players have a DISCONNECTED state
     *In short, sets gameover and return to waitStart routine
     */
    void _manage_not_enough_players();
};

#endif
