#include "GameStateWorker.hpp"

GameStateWorker::GameStateWorker(PlayingArea & playingArea,
				 QMutex & playingAreaMutex,
				 QVector<PlayerState> & playersStates,
				 QVector<QMutex*> & playersStatesMutexes,
				 GameState & gameState,
				 QMutex & gameStateMutex)
 :_playingArea(playingArea),
  _playersStates(playersStates),
  _gameState(gameState),
  _playingAreaMutex(playingAreaMutex),
  _playersStatesMutexes(playersStatesMutexes),
  _gameStateMutex(gameStateMutex)
{}

void GameStateWorker::_update_rackets()
{
  for(int i=0; i < _playersStates.size(); ++i)
    {
      QGraphicsLineItem racket;
      const QPointF playingAreaCenter(0,0);
      int nbPlayers = _playersStates.size();

      _playersStatesMutexes[i]->lock();
      racket.setLine( _playersStates[i].relativeRacket() );
      _playersStatesMutexes[i]->unlock();

      racket.setTransformOriginPoint( playingAreaCenter );
      racket.setRotation( i * (360/nbPlayers) );

      _playingAreaMutex.lock();
      _playingArea.setRacketCoord(i, racket.line().p1(), racket.line().p2() );
      _playingAreaMutex.unlock();
    }
}

void GameStateWorker::_check_collisions()
{
  int pos;

  _playingAreaMutex.lock();

  QList<QGraphicsItem*> colliders = _playingArea.getBallColliders();

  if( !colliders.empty() )
    {
      assert( colliders.size() == 1 );

      QGraphicsItem & collided = *colliders.at(0);
      

      if( ( pos = _playingArea.cageIndex(collided) ) !=  -1)
	{
	  _manage_goal(pos);
	}

      else if( ( pos = _playingArea.racketIndex(collided) ) != -1)
	{
	  _manage_racket_collision(pos);
	}

      else
	{
	  pos = _playingArea.wallIndex(collided);

	  _manage_wall_collision(pos);
	}
    }

  _playingAreaMutex.unlock();
}

void GameStateWorker::_manage_goal(const int & cageIndex)
{
  _playersStatesMutexes[cageIndex]->lock();
  _playingAreaMutex.lock();
  _gameStateMutex.lock();

  //decrease credits
  _playersStates[cageIndex].decreaseCredit();

  //discard player ?
  if( _playersStates[cageIndex].credit() == 0 )
    {
      if( _playersStates.size() > 2)
	{
	  _discard_player(cageIndex);

	  _playingArea.update();
	}

      else
	_gameState.setGameOverState(cageIndex);
    }
  
  //bring in a new ball
  if( !_game_over() )
    _playingArea.resetBall();

  _gameStateMutex.unlock();
  _playingAreaMutex.unlock();
  _playersStatesMutexes[cageIndex]->unlock();
}

void GameStateWorker::_discard_player(const int & racketIndex)
{
  _playingAreaMutex.lock();
  _playersStatesMutexes[racketIndex]->lock();

  //remove cage and racket
  _playingArea.removeCage(racketIndex);
  _playingArea.removeRacket(racketIndex);

  //set playerState to discarded
  _playersStates[racketIndex].setState(PongTypes::DISCARDED);

  _playersStatesMutexes[racketIndex]->unlock();
  _playingAreaMutex.unlock();
}

void GameStateWorker::_manage_wall_collision(const int & wallIndex)
{
  _playingAreaMutex.lock();

  qreal alpha = _playingArea.getWallRotation(wallIndex);

  // change referential to wall's referential
  //rotation of wall's angle
  // |cos(a) -sin(a) |
  // | sin(a) cos(a) |
  _playingArea.rotateBallDirection(alpha);

  //********** new direction
  //axial symetry with x axis:
  // |-1 0 |
  // | 0 1 |
  //use QTransform
  _playingArea.mirrorBallDirection(-1,0,0,1);

  //come back to ball's referential)
  //rotation of wall's angle
  // |cos(a) -sin(a) |
  // |sin(a)  cos(a) |
  //use QTransform
  _playingArea.rotateBallDirection(-alpha);

  _playingAreaMutex.unlock();
}

void GameStateWorker::_manage_racket_collision(const int & racketIndex)
{
  //for now, just do the same thing
  //may change later
  _manage_wall_collision(racketIndex);
}

void GameStateWorker::_move_ball()
{
  _playingAreaMutex.lock();

  //translate in relative referential
  _playingArea.moveBall();

  _playingAreaMutex.unlock();
}

bool GameStateWorker::_game_over()
{
  return _gameState.state() == PongTypes::GAMEOVER;
}

void GameStateWorker::_manage_game_over()
{
  //maybe some actions will be needed later
}

void GameStateWorker::checkState()
{
  while( !_game_over() )
    {
      _update_rackets();
      _check_collisions();

      if( !_game_over() )
	_move_ball();

      else
	_manage_game_over();
    }
}
