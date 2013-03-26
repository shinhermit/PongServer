#include "GameStateWorker.hpp"

GameStateWorker::GameStateWorker(PlayingArea & playingArea,
				 QMutex & playingAreaMutex,
				 Qvector<PlayerState> & playersStates,
				 Qvector<QMutex> & playersStatesMutexes,
				 const bool & stopped,
				 QMutex & stoppedMutex);

void GameStateWorker::_update_rackets()
{
}

void GameStateWorker::_check_collisions()
{
  int i pos;

  QList<QGraphicsItem*> colliders = _playingArea.ball().graphicItem().collidingItems();

  if( !colliders.empty() )
    {
      assert(colliders.size() == 1);

      QGraphicsItem & collided = *colliders.at(0);
      

      if( ( pos = _playingArea.cages().indexOf(collided) ) !=  -1)
	{
	  //change score (ie conceded goals)
	  //eventually discard player
	  //bring in a new ball (position+direction+speed)
	  _manage_goal();
	}

      else if( ( pos = _playingArea.rackets().indexOf(collided) ) != -1)
	{
	  //determine ball (position, direction, speed)
	  //eventually according to the the racket speed
	  _manage_racket_collision();
	}

      else
	{
	  //determine ball (position, direction)
	  _manage_wall_collision();
	}
    }
}

void GameStateWorker::_manage_goal()
{
}

void GameStateWorker::_manage_racket_collision()
{
}

void GameStateWorker::_manage_wall_collision()
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
