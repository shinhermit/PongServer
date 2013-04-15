#include "GameStateWorker.hpp"

GameStateWorker::GameStateWorker(
        GameState *gameState,
        PlayingArea *playingArea,
        QVector<PlayerState *> *playersStates,
        QMutex *playersStatesMutex
        ):
    _downCounter(-1),
    _playingArea(playingArea),
    _playersStates(playersStates),
    _playersStatesMutex(playersStatesMutex),
    _gameState(gameState)
{
    connect( this, SIGNAL(checkInitSignal()), this, SLOT(checkInitSlot()) );
    connect( this, SIGNAL(checkRunningSignal()), this, SLOT(checkRunningSlot()) );
}

void GameStateWorker::waitStartSlot()
{
    PongTypes::E_GameState state;

    //debug
    emit appendStatusSignal("GameStateWorker::waitStartSlot: Entering waitStart routine");

    _gameState->setWaitingServer();

    state = _gameState->state();

    while(state != PongTypes::START_REQUESTED && state != PongTypes::EXIT_REQUESTED)
    {
        state = _gameState->state();
    }

    //debug
    emit appendStatusSignal("GameStateWorker::waitStartSlot: getting out of waitStart routine");
    emit appendStatusSignal("GameStateWorker::waitStartSlot: emitting checkInitSignal");

    if(state != PongTypes::EXIT_REQUESTED)
        emit checkInitSignal();
    else
        emit finishedSignal();
}

void GameStateWorker::checkInitSlot()
{
    _gameState->setInitializing();

    _downCounter = 4;

    //debug
    emit appendStatusSignal("GameStateWorker::checkInitSlot: gameState set to INITIALIZING");
    emit appendStatusSignal("GameStateWorker::checkInitSlot: arming timer, with downCounter initialized to 4");

    if( !_exit_requested() )
    {
        connect( &_timer, SIGNAL(timeout()), this, SLOT(_countDownSlot()) );
        _timer.start(1000);
    }

    else
        emit finishedSignal();
}

void GameStateWorker::checkRunningSlot()
{
    //debug
    emit appendStatusSignal("GameStateWorker::checkRunningSlot: entering checkRunning routine");

    _gameState->setRunning();

    emit beginMovingBallSignal();

    while( !_game_over() && !_exit_requested() )
    {
        _update_rackets();
        _check_collisions();

        if( _game_over() )
            _manage_game_over();
    }

    //debug
    emit appendStatusSignal("GameStateWorker::checkRunningSlot: leaving checkRunning routine");

    if( _exit_requested() )
        emit finishedSignal();
}

void GameStateWorker::_countDownSlot()
{
    if( _downCounter > 0 && !_exit_requested() )
    {
        -- _downCounter;

        _gameState->setDownCounter(_downCounter);
    }

    else
    {
        _timer.stop();

        if( !_exit_requested() )
            emit checkRunningSignal();
        else
            emit finishedSignal();
    }
}

bool GameStateWorker::_exit_requested()
{
    bool requested;

    requested  = ( _gameState->state() == PongTypes::EXIT_REQUESTED );

    return requested;
}

void GameStateWorker::_update_rackets()
{
    QVector<qreal> dx;
    int i;

    for(i=0; i < _playersStates->size(); ++i)
    {
        dx.push_back( _playersStates->at(i)->dxRacket() );
    }

    for(int i=0; i < dx.size(); ++i)
    {
        _playingArea->moveRacket( i, dx[i] );
    }
}

void GameStateWorker::_check_collisions()
{
    int pos;

    QList<QGraphicsItem*> colliders = _playingArea->getBallColliders();

    if( !colliders.empty() )
    {
        QGraphicsItem * collided = colliders.at(0);


        if( ( pos = _playingArea->cageIndex(collided) ) !=  -1)
        {
            _manage_goal(pos);
        }

        else if( ( pos = _playingArea->racketIndex(collided) ) != -1)
        {
            _manage_racket_collision(pos);
        }

        else
        {
            pos = _playingArea->wallIndex(collided);

            _manage_wall_collision(pos);
        }
    }
}

void GameStateWorker::_manage_goal(const int & cageIndex)
{
    //debug
    emit appendStatusSignal("GameStateWorker::_manage_goal: player "+QString::number(cageIndex)+"  conceded a goal");

    //decrease credits
    _playersStates->at(cageIndex)->decreaseCredit();

    //discard player ?
    if( _playersStates->at(cageIndex)->credit() == 0 )
    {
        if( _playersStates->size() > 2)
            _discard_player(cageIndex);

        else
            _gameState->setGameOver(cageIndex);
    }

    //bring in a new ball
    if( !_game_over() )
        _playingArea->resetBallPos();
}

void GameStateWorker::_discard_player(const int & racketIndex)
{
    //remove cage and racket
    _playingArea->removeCage(racketIndex);
    _playingArea->removeRacket(racketIndex);

    //set playerState to discarded
    _playersStates->at(racketIndex)->setState(PongTypes::DISCARDED);

    //debug
    emit appendStatusSignal("GameStateWorker::_discard_player: player "+QString::number(racketIndex)+"  discarded");
}

void GameStateWorker::_manage_wall_collision(const int & wallIndex)
{
    _playingArea->mirrorBallDirection( _playingArea->wall(wallIndex) );
}

void GameStateWorker::_manage_racket_collision(const int & racketIndex)
{
    //for now, just do the same thing
    //may change later
    _manage_wall_collision(racketIndex);
}

bool GameStateWorker::_game_over()
{
    return _gameState->state() == PongTypes::GAMEOVER;
}

void GameStateWorker::_manage_game_over()
{
    //debug
    emit appendStatusSignal("GameStateWorker::_manage_game_over: game over state detected");

    //signal for PongServer::newGame Slot
    emit gameOverSignal();
}
