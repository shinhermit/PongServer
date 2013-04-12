#ifndef _GameStateWorker
#define _GameStateWorker

#include <cassert>

#include <QObject>
#include <QList>
#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QLineF>
#include <QPointF>
#include <QVector>
#include <QTimer>

#include "PlayingArea.hpp"
#include "PlayerState.hpp"
#include "GameState.hpp"
#include "PongTypes.hpp"
#include "PongServerView.hpp"

class GameStateWorker : public QObject
{
    Q_OBJECT

public:
    GameStateWorker(PongServerView & view,
            PlayingArea & playingArea,
            QVector<PlayerState*> & playersStates,
            QMutex & playersStatesMutex,
            GameState & gameState
            );

signals:
    void checkInitSignal();
    void checkRunningSignal();

public slots:
    void waitStartSlot();
    void checkInitSlot();
    void checkRunningSlot();

private slots:
    void _countDownSlot();

private:
    QTimer _timer;
    qint32 _downCounter;

    PongServerView & _view;
    PlayingArea & _playingArea;
    QVector<PlayerState*> & _playersStates;
    QMutex & _playersStatesMutex;
    GameState & _gameState;

    void _update_rackets();

    void _check_collisions();

    void _manage_goal(const int & cageIndex);

    void _discard_player(const int & racketIndex);

    void _manage_wall_collision(const int & wallIndex);

    void _manage_racket_collision(const int & racketIndex);

    bool _game_over();

    void _manage_game_over();

    void _move_ball();
};

/*!
 *\class GameStateWorker GameStateWorker.hpp "headers/GameStateWorker.hpp"
 * \brief The GameStateWorker class is mean to be runned by a thread
 *This class checks shared memories and update the game state (rackets positions, game over, discard player, etc.)
 */

/*!
* \fn GameStateWorker
* \param playingArea
* \param playingAreaMutex
* \param playersStates
* \param playersStatesMutexes
* \param gameState
* \param gameStateMutex
*
* Constructor
*/

/*!
 * \fn _update_rackets
 * read rackets delta(x)
 * update playingArea
 */

/*!
 * \fn _check_collisions
 */

/*!
 * \fn _manage_goal
 * \param cageIndex
 * change credits (ie conceded goals)
 * eventually discard player
 *bring in a new ball (position+direction+speed)
 */

/*!
 * \fn _discard_player
 * \param racketIndex
 * remove cage (+ racket ?) of the player
 * put player in discarded state
 */

/*!
 * \fn _manage_wall_collision
 * \param wallIndex
 *determine ball (position, direction)
 */

/*!
 * \fn _manage_racket_collision
 * \param racketIndex
 * determine ball (position, direction, speed)
 * eventually according to the the racket speed
 */

/*!
 * \fn _game_over
 * \return true if the state of the game is PongTypes::GAMEOVER
 */

/*!
 * \fn _manage_game_over
 * planned for any action eventually needed in a game over state
 */

/*!
 * \fn _move_ball
 * just moves the ball in the direction of
 * ball _playingArea.ball().direction()
 */

#endif
