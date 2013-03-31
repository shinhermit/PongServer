#include "GameStateWorker.hpp"

GameStateWorker::GameStateWorker(
        PlayingArea & playingArea,
        QVector<PlayerState*> & playersStates,
        QMutex & playersStatesMutex,
        GameState & gameState
        ):
    _downCounter(3),
    _playingArea(playingArea),
    _playersStates(playersStates),
    _playersStatesMutex(playersStatesMutex),
    _gameState(gameState)
{
    QObject::connect( &_timer, SIGNAL(timeout()), this, SLOT(_count_down()) );
    QObject::connect( this, SIGNAL(_checkRunningSignal()), this, SLOT(_countDownSlot()) );
}

void GameStateWorker::_update_rackets()
{
    _playingArea.lock();
    for(int i=0; i < _playersStates.size(); ++i)
    {
        _playersStates[i]->lock();

        _playingArea.moveRacket( i, _playersStates[i]->dxRacket() );

        _playersStates[i]->unlock();
    }
    _playingArea.unlock();
}

void GameStateWorker::_check_collisions()
{
    int pos;

    _playingArea.lock();

    QList<QGraphicsItem*> colliders = _playingArea.getBallColliders();

    if( !colliders.empty() )
    {
        QGraphicsItem * collided = colliders.at(0);


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

    _playingArea.unlock();
}

void GameStateWorker::_manage_goal(const int & cageIndex)
{
    _playersStates[cageIndex]->lock();
    _playingArea.lock();
    _gameState.lock();

    //decrease credits
    _playersStates[cageIndex]->decreaseCredit();

    //discard player ?
    if( _playersStates[cageIndex]->credit() == 0 )
    {
        if( _playersStates.size() > 2)
            _discard_player(cageIndex);

        else
            _gameState.setGameOver(cageIndex);
    }

    //bring in a new ball
    if( !_game_over() )
        _playingArea.resetBallPos();

    _gameState.unlock();
    _playingArea.unlock();
    _playersStates[cageIndex]->unlock();
}

void GameStateWorker::_discard_player(const int & racketIndex)
{
    _playingArea.lock();
    _playersStates[racketIndex]->lock();

    //remove cage and racket
    _playingArea.removeCage(racketIndex);
    _playingArea.removeRacket(racketIndex);

    //set playerState to discarded
    _playersStates[racketIndex]->setState(PongTypes::DISCARDED);

    _playersStates[racketIndex]->unlock();
    _playingArea.unlock();
}

void GameStateWorker::_manage_wall_collision(const int & wallIndex)
{
    _playingArea.lock();

    _playingArea.mirrorBallDirection( _playingArea.wall(wallIndex) );

    _playingArea.unlock();
}

void GameStateWorker::_manage_racket_collision(const int & racketIndex)
{
    //for now, just do the same thing
    //may change later
    _manage_wall_collision(racketIndex);
}

void GameStateWorker::_move_ball()
{
    _playingArea.lock();

    //translate in relative referential
    _playingArea.moveBall();

    _playingArea.unlock();
}

bool GameStateWorker::_game_over()
{
    return _gameState.state() == PongTypes::GAMEOVER;
}

void GameStateWorker::_manage_game_over()
{
    //maybe some actions will be needed later
}

void GameStateWorker::checkInitSlot()
{
    bool ready;

    ready = false;
    while( !ready )
    {
        _playersStatesMutex.lock();

        for(int i=0; i<_playersStates.size() && _playersStates.size() > 1 && ready; ++i)
        {
            ready = (_playersStates[i]->state() == PongTypes::READY);
        }

        if(ready)
        {
            _gameState.lock();
            _gameState.setInitializing();
            _gameState.unlock();
        }

        _playersStatesMutex.unlock();
    }

    _downCounterText = new QGraphicsTextItem( QString::number(_downCounter) );

    _playingArea.lock();
    _playingArea.scene()->addItem(_downCounterText);
    _playingArea.unlock();

    _timer.start(1000);
}

void GameStateWorker::checkRunningSlot()
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

void GameStateWorker::_countDownSlot()
{
    if(_downCounter > 0)
    {
        -- _downCounter;
        _downCounterText->setPlainText( QString::number(_downCounter) );
    }

    else
    {
        _timer.stop();

        _playingArea.lock();
        _playingArea.scene()->removeItem(_downCounterText);
        _playingArea.unlock();

        _gameState.lock();
        _gameState.setRunning();
        _gameState.unlock();

        emit checkRunningSignal();
    }
}


