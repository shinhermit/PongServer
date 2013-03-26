#include "GameStateWorker.hpp"

GameStateWorker::GameStateWorker(PlayingArea & playingArea,
				 QMutex & playingAreaMutex,
				 Qvector<PlayerState> & playersStates,
				 Qvector<QMutex> & playersStatesMutexes,
				 const bool & stopped,
				 QMutex & stoppedMutex);

void GameStateWorker::_update_rackets()
{
  for(int i=0; i < _playersStates.size(); ++i)
    {
      QLine racket;
      QPointF & playinAreaCenter = _playingArea.centerPoint();
      int & nbPlayers = playersStates.size();

      _playersStatesMutexes[i].lock();
      QLine racket = _playersStates[i].relativeRacket();
      _playersStatesMutexes[i].unlock();

      racket.setTransformOriginPoint( playingAreaCenter );
      racket.setRotation( i * (360/nbPlayers) );

      _playingAreaMutex.lock();
      _playingArea.racket(i).setP1( racket.p1() );
      _playingArea.racket(i).setP2( racket.p2() );
      _playingAreaMutex.unlock();
    }
}

void GameStateWorker::_check_collisions()
{
  int i pos;

  _playingAreaMutex.lock();

  QList<QGraphicsItem*> colliders = _playingArea.ball().graphicsItem().collidingItems();

  if( !colliders.empty() )
    {
      assert(colliders.size() == 1);

      QGraphicsItem & collided = *colliders.at(0);
      

      if( ( pos = _playingArea.cages().indexOf(collided) ) !=  -1)
	{
	  _manage_goal(pos);
	}

      else if( ( pos = _playingArea.rackets().indexOf(collided) ) != -1)
	{
	  _manage_racket_collision(pos);
	}

      else
	{
	  pos = _playingArea.walls().indexOf(collided);

	  _manage_wall_collision(pos);
	}
    }

  _playingAreaMutex.unlock();
}

void GameStateWorker::_manage_goal(const int & cageIndex)
{
  _playersStatesMutexes[cageIndex].lock();
  _playingAreaMutex.lock();
  _gameStateMutex.lock();

  //decrease credits
  _playersStates[cageIndex].decreaseCredits();

  //discard player ?
  if( _playersStates[cageIndex].credits() == 0 )
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
  _playersStatesMutexes[cageIndex].unlock();
}

void GameStateWorker::_discard_player(const int & racketIndex)
{
  _playingAreaMutex.lock();
  _playersStatesMutexes[racketIndex].lock();

  //remove cage and racket
  _playingArea.removeCage(racketIndex);
  _playingArea.removeRacket(racketIndex);

  //set playerState to discarded
  _playersStates[racketIndex].setState(PongTypes::DISCARDED);

  _playersStatesMutexes[racketIndex].unlock();
  _playingAreaMutex.unlock();
}

void GameStateWorker::_manage_wall_collision(const int & wallIndex)
{
  //new direction
  

  //new position
}

void GameStateWorker::_manage_racket_collision(const int & racketIndex)
{
}

void GameStateWorker::_move_ball()
{
}

bool GameStateWorker::_game_over()
{
}

void GameStateWorker::_manage_game_over()
{
}

void GameStateWorker::checkState()
{
  _update_rackets();
  _check_collisions();

  if( !_game_over() )
    _move_ball();

  else
    _manage_game_over();
}
