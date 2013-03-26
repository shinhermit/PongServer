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

class GameStateWorker : public QObject
{
  Q_OBJECT

  public:
  GameStateWorker(PlayingArea & playingArea,
		  QMutex & playingAreaMutex,
		  Qvector<PlayerState> & playersStates,
		  Qvector<QMutex> & playersStatesMutexes,
		  const bool & stopped,
		  QMutex & stoppedMutex);

public slots:
  void checkState();

private:
  PlayingArea & _playingArea;
  Qvector<PlayerState> & _playersStates;
  const bool & _stopped;

  QMutex & _playingAreaMutex;
  Qvector<QMutex> & _playersStatesMutexes;
  QMutex & _stoppedMutex;

  //read rackets delta(x)
  //update playingArea
  void _update_rackets();

  void _check_collisions();

  void _manage_goal();

  void _manage_racket_collision();

  void _manage_wall_collision();

  bool _game_over();

    void _manage_game_over();

  //just moves the ball in the direction of
  //ball _playingArea.ball().direction()
  void _move_ball();
}

#endif
