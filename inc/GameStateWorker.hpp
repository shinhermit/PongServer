#ifndef _GameStateWorker
#define _GameStateWorker

#include <cassert>

#include <QObject>
#include <QList>
#include <QGraphicsItem>
#include <QPointF>
#include <QVector>
#include <QMutex>

#include "PlayingArea.hpp"
#include "PlayerState.hpp"
#include "PongTypes.hpp"

class GameStateWorker : public QObject
{
  Q_OBJECT

  public:
  GameStateWorker(PlayingArea & playingArea,
		  QMutex & playingAreaMutex,
		  Qvector<PlayerState> & playersStates,
		  Qvector<QMutex> & playersStatesMutexes,
		  GameState & gameState,
		  QMutex & gameState);

public slots:
  void checkState();

private:
  PlayingArea & _playingArea;
  Qvector<PlayerState> & _playersStates;
  GameState & _gameState;

  QMutex & _playingAreaMutex;
  Qvector<QMutex> & _playersStatesMutexes;
  QMutex & _gameStateMutex;

  //read rackets delta(x)
  //update playingArea
  void _update_rackets();

  void _check_collisions();

  //change credits (ie conceded goals)
  //eventually discard player
  //bring in a new ball (position+direction+speed)
  void _manage_goal(const int & cageIndex);

  //=> remove cage (+ racket ?) of the player
  //put player in discarded state
  void _discard_player(const int & racketIndex);

  //determine ball (position, direction)
  void _manage_wall_collision(const int & wallIndex);

  //determine ball (position, direction, speed)
  //eventually according to the the racket speed
  void _manage_racket_collision(const int & racketIndex);

  bool _game_over();

  void _manage_game_over();

  //just moves the ball in the direction of
  //ball _playingArea.ball().direction()
  void _move_ball();
}

#endif
