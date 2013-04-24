#include "PongServer.hpp"

PongServer::PongServer(const qint16 & port):
    _playerLogger(port)
{
    PongShared::gameState.setNoParty();
    PongShared::playingArea.build();

    connect( this, SIGNAL(newGameSignal()), this, SLOT(newGameSlot()) );

    connect( this, SIGNAL(startService()), &_gameStateChecker, SLOT(waitStartSlot()) );
    connect( &_gameStateChecker, SIGNAL(finishedSignal()), &_gameStateCheckerThread, SLOT(quit()) );
    connect( &_gameStateChecker, SIGNAL(appendStatusSignal(QString)), &_view, SLOT(appendStatusSlot(QString)) );
    connect( &_gameStateCheckerThread, SIGNAL(finished()), this, SLOT(threadTerminated()) );
    connect( this, SIGNAL(stopService()), &_gameStateChecker, SLOT(notBusyQuit()) );
    connect( &_gameStateChecker, SIGNAL(gameOverSignal()), this, SLOT(gameOverSlot()) );

    connect( &_gameStateChecker, SIGNAL(startMovingBall()), &_ballMover, SLOT(startMoving()) );
    connect( &_gameStateChecker, SIGNAL(stopMovingBall()), &_ballMover, SLOT(stopMoving()) );
    connect( &_ballMover, SIGNAL(appendStatusSignal(QString)), &_view, SLOT(appendStatusSlot(QString)) );
    connect( &_ballMover, SIGNAL(finishedSignal()), &_ballMoverThread, SLOT(quit()) );
    connect( this, SIGNAL(stopService()), &_ballMover, SLOT(notBusyQuit()) );
    connect( &_ballMoverThread, SIGNAL(finished()), this, SLOT(threadTerminated()) );

    _gameStateChecker.moveToThread(&_gameStateCheckerThread);
    _ballMover.moveToThread(&_ballMoverThread);

    _gameStateCheckerThread.start();
    _ballMoverThread.start();

    connect( this, SIGNAL(startService()), &_playerLogger, SLOT(waitConnections()) );
    connect( &_playerLogger, SIGNAL(appendStatusSignal(QString)), &_view, SLOT(appendStatusSlot(QString)) );
    connect( &_playerLogger, SIGNAL(newPlayerConnected()), this, SLOT(newPlayerConnected()) );


    connect(&_view, SIGNAL(startClickedSignal()), this, SLOT(startRequestedSlot()) );
    connect(&_view, SIGNAL(exitSignal()), this, SLOT(quitSlot()) );

    //debug
    _view.appendStatus("PongServer::PongServer : Server Active; GameState set to NOPARTY, gameStatechecker started");
}

void PongServer::start()
{
    _view.show();

    emit newGameSignal();
}

void PongServer::showScene()
{
    _graphicScene.addItem( PongShared::playingArea.scene() );
    _graphicView.setScene(&_graphicScene);
    _graphicView.show();
}

void PongServer::gameStateErrorSlot(const QString &mess)
{
    //when not enough players (only 1)
    lockGameState();
    PongShared::gameState.setStateError();
    unlockGameState();

    qDebug() << mess << endl;
}

void PongServer::newPlayerConnected()
{
    qint32 nbPlayers;

    //debug
    _view.appendStatus("PongServer::newPlayersConnected : signal received");

    lockGameState();
    nbPlayers = PongShared::gameState.nbPlayers();
    unlockGameState();

    if(nbPlayers > 1)
        _view.enableStartButton();
}

void PongServer::startRequestedSlot()
{
    qint32 nbPlayers;
    QVector<QLineF> rackets;

    lockGameState();
    PongShared::gameState.setStartRequested();
    nbPlayers = PongShared::gameState.nbPlayers();
    unlockGameState();

    lockPlayingArea();
    PongShared::playingArea.rebuild(nbPlayers);
    for(int i=0; i < PongShared::playingArea.nbRackets(); ++i)
        rackets.push_back( PongShared::playingArea.racketLine(i) );
    unlockPlayingArea();

    lockPlayersStates();
    for(int i=0; i < PongShared::playersStates.size(); ++i)
        PongShared::playersStates[i].setRacket(rackets[i]);
    unlockPlayersStates();

    //debug
    _view.appendStatus("PongServer::startRequestedSlot: gameState set to START_REQUESTED");
}

void PongServer::gameOverSlot()
{
    QTimer::singleShot( 500, this, SLOT(newGameSlot()) );
}

void PongServer::quitSlot()
{
    //debug
    _view.appendStatus("PongServer::quitSlot: quitSignal received");
    _view.disableStartButton();

    lockGameState();
    PongShared::gameState.setExitRequested();
    unlockGameState();

    emit stopService();
}

void PongServer::threadTerminated()
{
    _view.appendStatus("PongServer::threadTerminated : a thread has terminated");

    if( _all_threads_finished() )
    {
        _view.appendStatus("PongServer::threadTerminated : All thread have terminated");
        _view.disableQuitButton();
    }
}

void PongServer::newGameSlot()
{
    //debug
    _view.appendStatus("PongServer::newGameSlot : resetting Game");

    _view.disableStartButton();

    //delete workers and thread for disconnected players

    lockPlayersStates();
    if(PongShared::playersStates.size() > 0)
    {
        int i=0;
        while( i < PongShared::playersStates.size() )
        {
            PongShared::playersStates[i].setId(i);

            if( PongShared::playersStates[i].state() == PongTypes::DISCONNECTED )
                PongShared::playersStates.erase( PongShared::playersStates.begin()+i );

            else
                ++i;
        }
    }

    if( PongShared::playersStates.size() > 1 )
        _view.enableStartButton();

    else
        _view.disableStartButton();

    unlockPlayersStates();


    //reset gameState, playersStates
    _reset_gameState();
    _reset_playersStates();

    //start workers
    emit startService();

    //debug
    _view.appendStatus("PongServer::newGameSlot : GameState reset; startService signal sent");
}

void PongServer::_reset_gameState()
{
    qint32 nbPlayers;

    lockPlayersStates();
    nbPlayers = PongShared::playersStates.size();
    unlockPlayersStates();

    lockGameState();
    PongShared::gameState.setWaitingServer();
    PongShared::gameState.setNbPlayers(nbPlayers);
    PongShared::gameState.setNbActive(nbPlayers);
    PongShared::gameState.setLoserIndex(-1);
    PongShared::gameState.setDownCounter(0);
    unlockGameState();
}

void PongServer::_reset_playersStates()
{
    lockPlayersStates();
    for(int i=0; i < PongShared::playersStates.size(); ++i)
    {
        PongShared::playersStates[i].setState(PongTypes::ACCEPTED);
        PongShared::playersStates[i].setCredit( PlayerState::DefaultCredit() );
    }
    unlockPlayersStates();
}

bool PongServer::_all_threads_finished()
{
    return ( _gameStateCheckerThread.isFinished() && _ballMoverThread.isFinished() );
}
