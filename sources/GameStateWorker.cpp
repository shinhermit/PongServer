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

    //debug
    qDebug() << "GameStateWorker::waitStartSlot : nbPlayers="
             << PongShared::gameState.nbPlayers()
             << ", nbActive=" << PongShared::gameState.nbActive();

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
    //debug
    qDebug() << "GameStateWorker::waitStartSlot : nbPlayers="
             << PongShared::gameState.nbPlayers()
             << ", nbActive=" << PongShared::gameState.nbActive();
    unlockGameState();

    _downCounter = 4;

    //debug
    emit appendStatusSignal("GameStateWorker::checkInitSlot: gameState set to INITIALIZING");

    if( !_exit_requested() )
    {
        if( _enough_players() ) //some players has been disconnected ?
        {
            //debug
            emit appendStatusSignal("GameStateWorker::checkInitSlot: arming timer, with downCounter initialized to 4");
            _timer.start(1000);
        }

        else
            _manage_not_enough_players();
    }

    else
        emit finishedSignal();
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

void GameStateWorker::checkRunningSlot()
{
    //debug
    emit appendStatusSignal("GameStateWorker::checkRunningSlot: entering checkRunning routine");

    lockGameState();
    PongShared::gameState.setRunning();
    unlockGameState();

    //debug
    emit appendStatusSignal("GameStateWorker::checkInitSlot: gameState set to RUNNING");

    if( _enough_players() )
    {
        emit startMovingBall();
        _check_running_routine();
    }

    else
        _manage_not_enough_players();
}

void GameStateWorker::notBusyQuit()
{
    if( _timer.isActive() )
        _timer.stop();

    emit finishedSignal();
}

void GameStateWorker::_check_running_routine()
{
    _update_rackets();
    _check_collisions();

    if( _game_over() )
    {
        _manage_game_over();
    }

    //routine
    else if( !_exit_requested() )
    {
        if( _enough_players() )
            QTimer::singleShot( 3, this, SLOT(_check_running_routine()) );

        else
            _manage_not_enough_players();
    }

    else
    {
        //debug
        emit appendStatusSignal("GameStateWorker::checkRunningSlot: leaving checkRunning routine");

        emit stopMovingBall();
        emit finishedSignal();
    }
}

void GameStateWorker::_update_rackets()
{
    QVector<QLineF> rackets;
    int i;

    lockPlayersStates();
    for(i=0; i < PongShared::playersStates.size(); ++i)
        rackets.push_back( PongShared::playersStates[i].racket() );
    unlockPlayersStates();

    lockPlayingArea();
    i=0;
    while( i < PongShared::playingArea.nbRackets() && i < rackets.size()  )
    {
        if( rackets[i] != PongShared::playingArea.racketLine(i) )
            PongShared::playingArea.setRacketLine(i, rackets[i]);
        ++i;
    }
    unlockPlayingArea();
}

void GameStateWorker::_check_collisions()
{
    int cageIndex, racketIndex, wallIndex;
    bool collisionHappened;
    PlayingArea::Linear * collider = 0;

    lockPlayingArea();
    collisionHappened = PongShared::playingArea.collisionHappened();
    collider = PongShared::playingArea.getBallCollider();
    cageIndex = PongShared::playingArea.cageIndex(collider);
    racketIndex = PongShared::playingArea.racketIndex(collider);
    wallIndex = PongShared::playingArea.wallIndex(collider);
    unlockPlayingArea();

    if(collisionHappened)
    {
        if(cageIndex !=  -1)
        {
            _manage_goal(cageIndex);
        }

        else if( racketIndex != -1)
        {
            _manage_racket_collision(racketIndex);
        }

        else
        {
            _manage_wall_collision(wallIndex);
        }
    }
}

void GameStateWorker::_manage_goal(const int & cageIndex)
{
    qint32 credit, nbActive;

    emit stopMovingBall();

    lockPlayersStates();
    PongShared::playersStates[cageIndex].decreaseCredit();
    credit = PongShared::playersStates[cageIndex].credit();
    unlockPlayersStates();

    lockGameState();
    nbActive = PongShared::gameState.nbActive();
    unlockGameState();

    //discard player ?
    if( credit == 0 )
    {
        if(nbActive > 2)
            _discard_player(cageIndex);

        else
        {
            qDebug() << "GameStateWorker::_manage_goal : nbActive = " << nbActive;
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
        emit checkInitSignal();
    }

    emit appendStatusSignal("GameStateWorker::_manage_goal: player "+QString::number(cageIndex)+" conceded a goal");
}

void GameStateWorker::_discard_player(const int & racketIndex)
{
    lockPlayersStates();
    PongShared::playersStates[racketIndex].setState(PongTypes::DISCARDED);
    PongShared::playersStates[racketIndex].setRacket(500,500,500,500);
    unlockPlayersStates();

    lockPlayingArea();
    PongShared::playingArea.removeCage(racketIndex);
    PongShared::playingArea.setRacketLine(racketIndex, QLineF(500,500,500,500));
    qDebug() << "GameStateWorker::_discard_player(" << racketIndex << "): removed cage and discarded racket";
    unlockPlayingArea();

    lockGameState();
    PongShared::gameState.decNbActive();
    unlockGameState();

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

bool GameStateWorker::_enough_players()
{
    bool enough = false;

    lockGameState();
    enough = ( PongShared::gameState.nbPlayers() >= 2 );
    unlockGameState();

    return enough;
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

    emit stopMovingBall();
    emit gameOverSignal();
}

void GameStateWorker::_manage_not_enough_players()
{
    lockGameState();
    PongShared::gameState.setGameOver(-1);
    unlockGameState();

    //debug
    emit appendStatusSignal("GameStateWorker::_manage_not_enough_players(): too few players remaining");

    _manage_game_over();
}

bool GameStateWorker::_exit_requested()
{
    bool requested;

    lockGameState();
    requested  = ( PongShared::gameState.state() == PongTypes::EXIT_REQUESTED );
    unlockGameState();

    return requested;
}
