#include "GameStateWorker.hpp"

GameStateWorker::GameStateWorker(QObject *parent):
    QObject(parent),
    _downCounter(-1)
{
    connect( &_timer, SIGNAL(timeout()), this, SLOT(_countDownSlot()) );
    connect( this, SIGNAL(checkInitSignal()), this, SLOT(checkInitSlot()) );
    connect( this, SIGNAL(checkRunningSignal()), this, SLOT(checkRunningSlot()) );
}

void GameStateWorker::waitStartSlot()
{
    PongTypes::E_GameState state;

    //debug
    emit appendStatusSignal("GameStateWorker::waitStartSlot: Entering waitStart routine");

    lockGameState();
    PongShared::gameState.setWaitingServer();

    state = PongShared::gameState.state();
    unlockGameState();

    while(state != PongTypes::START_REQUESTED && state != PongTypes::EXIT_REQUESTED)
    {
        lockGameState();
        state = PongShared::gameState.state();
        unlockGameState();
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
    lockGameState();
    PongShared::gameState.setInitializing();
    unlockGameState();

    _downCounter = 4;

    //debug
    emit appendStatusSignal("GameStateWorker::checkInitSlot: gameState set to INITIALIZING");
    emit appendStatusSignal("GameStateWorker::checkInitSlot: arming timer, with downCounter initialized to 4");

    if( !_exit_requested() )
        _timer.start(1000);
    else
        emit finishedSignal();
}

void GameStateWorker::checkRunningSlot()
{
    //debug
    emit appendStatusSignal("GameStateWorker::checkRunningSlot: entering checkRunning routine");

    lockGameState();
    PongShared::gameState.setRunning();
    unlockGameState();

    emit appendStatusSignal("GameStateWorker::checkInitSlot: gameState set to RUNNING");

    emit startMovingBall();

    _check_running_routine();
}

void GameStateWorker::_countDownSlot()
{
    if( _downCounter > 0 && !_exit_requested() )
    {
        -- _downCounter;

        lockGameState();
        PongShared::gameState.setDownCounter(_downCounter);
        unlockGameState();
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

void GameStateWorker::_check_running_routine()
{
    _update_rackets();
    _check_collisions();

    if( _game_over() )
    {
        _manage_game_over();
        emit stopMovingBall();
    }

    //routine
    else if( !_exit_requested() )
        QTimer::singleShot( 3, this, SLOT(_check_running_routine()) );

    else
    {
        //debug
        emit appendStatusSignal("GameStateWorker::checkRunningSlot: leaving checkRunning routine");

        emit stopMovingBall();
        emit finishedSignal();
    }
}

bool GameStateWorker::_exit_requested()
{
    bool requested;

    lockGameState();
    requested  = ( PongShared::gameState.state() == PongTypes::EXIT_REQUESTED );
    unlockGameState();

    return requested;
}

void GameStateWorker::_update_rackets()
{
    QVector<qreal> dx;

    lockPlayersStates();
    for(int i=0; i < PongShared::playersStates.size(); ++i)
        dx.push_back( PongShared::playersStates[i].dxRacket() );
    unlockPlayersStates();

    lockPlayingArea();
    for(int i=0; i < PongShared::playingArea.nbRackets() && i < dx.size(); ++i)
        PongShared::playingArea.moveRacket( i, dx[i] );
    unlockPlayingArea();
}

void GameStateWorker::_check_collisions()
{
    int pos;

    lockPlayingArea();

    QList<QGraphicsItem*> colliders = PongShared::playingArea.getBallColliders();

    if( !colliders.empty() )
    {
        QGraphicsItem * collided = colliders.at(0);

        if( ( pos = PongShared::playingArea.cageIndex(collided) ) !=  -1)
        {
            _manage_goal(pos);
        }

        else if( ( pos = PongShared::playingArea.racketIndex(collided) ) != -1)
        {
            _manage_racket_collision(pos);
        }

        else
        {
            pos = PongShared::playingArea.wallIndex(collided);

            _manage_wall_collision(pos);
        }
    }

    unlockPlayingArea();
}

void GameStateWorker::_manage_goal(const int & cageIndex)
{
    qint32 credit, nbPlayers;

    //debug
    emit appendStatusSignal("GameStateWorker::_manage_goal: player "+QString::number(cageIndex)+"  conceded a goal");

    lockPlayersStates();
    //decrease credits
    PongShared::playersStates[cageIndex].decreaseCredit();
    credit = PongShared::playersStates[cageIndex].credit();
    nbPlayers = PongShared::playersStates.size();
    unlockPlayersStates();

    //discard player ?
    if( credit == 0 )
    {
        if(nbPlayers > 2)
            _discard_player(cageIndex);

        else
        {
            lockGameState();
            PongShared::gameState.setGameOver(cageIndex);
            unlockGameState();
        }
    }

    //bring in a new ball
    if( !_game_over() )
    {
        lockPlayingArea();
        PongShared::playingArea.resetBallPos();
        unlockPlayingArea();
    }
}

void GameStateWorker::_discard_player(const int & racketIndex)
{
    lockPlayersStates();
    //set playerState to discarded
    PongShared::playersStates[racketIndex].setState(PongTypes::DISCARDED);
    unlockPlayersStates();

    //remove cage and racket
    lockPlayingArea();
    PongShared::playingArea.removeCage(racketIndex);
    PongShared::playingArea.removeRacket(racketIndex);
    unlockPlayingArea();

    //debug
    emit appendStatusSignal("GameStateWorker::_discard_player: player "+QString::number(racketIndex)+"  discarded");
}

void GameStateWorker::_manage_wall_collision(const int & wallIndex)
{
    lockPlayingArea();
    PongShared::playingArea.mirrorBallDirection( PongShared::playingArea.wall(wallIndex) );
    unlockPlayingArea();
}

void GameStateWorker::_manage_racket_collision(const int & racketIndex)
{
    lockPlayingArea();
    PongShared::playingArea.mirrorBallDirection( PongShared::playingArea.racket(racketIndex) );
    unlockPlayingArea();
}

void GameStateWorker::_move_ball()
{
    lockPlayingArea();
    //translate in relative referential
    PongShared::playingArea.moveBall();
    unlockPlayingArea();
}

bool GameStateWorker::_game_over()
{
    bool isOver;

    lockGameState();
    isOver = ( PongShared::gameState.state() == PongTypes::GAMEOVER );
    unlockGameState();

    return isOver;
}

void GameStateWorker::_manage_game_over()
{
    //debug
    emit appendStatusSignal("GameStateWorker::_manage_game_over: game over state detected");

    //maybe some actions will be needed later
    emit gameOverSignal();
}
