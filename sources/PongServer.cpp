#include "PongServer.hpp"

PongServer::PongServer(const int & maxPlayers,
                       const int & renderAreaWidth,
                       const qint16 & port):
    _maxPlayers(maxPlayers),
    _gameState(PongTypes::NOPARTY),
    _playingArea(maxPlayers, renderAreaWidth),
    _gameStateChecker(&_gameState, &_playingArea, &_playersStates, &_playersStatesMutex),
    _ballMover(&_gameState, &_playingArea),
    _playerLogger(_gameState, _playingArea, _playersStates, _playersStatesMutex, _socketWorkers, port)
{
    connect( this, SIGNAL(newGameSignal()), this, SLOT(newGameSlot()) );

    connect( this, SIGNAL(startService()), &_gameStateChecker, SLOT(waitStartSlot()) );
    connect( &_gameStateChecker, SIGNAL(finishedSignal()), &_gameStateCheckerThread, SLOT(quit()) );
    connect( &_gameStateChecker, SIGNAL(appendStatusSignal(QString)), &_view, SLOT(appendStatusSlot(QString)) );
    connect( &_gameStateCheckerThread, SIGNAL(finished()), this, SLOT(threadTerminated()) );
    connect( &_gameStateChecker, SIGNAL(gameOverSignal()), this, SLOT(newGameSlot()) );

    _gameStateChecker.moveToThread(&_gameStateCheckerThread);
    _gameStateCheckerThread.start();

    connect( &_gameStateChecker, SIGNAL(beginMovingBallSignal()), &_ballMover, SLOT(beginMovingBall()) );
    connect( &_ballMover, SIGNAL(finishedSignal()), &_ballMoverThread, SLOT(quit()) );
    connect( &_ballMoverThread, SIGNAL(finished()), this, SLOT(threadTerminated()) );

    _ballMover.moveToThread(&_ballMoverThread);
    _ballMoverThread.start();

    connect( this, SIGNAL(startService()), &_playerLogger, SLOT(waitConnections()) );
    connect( &_playerLogger, SIGNAL(appendStatusSignal(QString)), &_view, SLOT(appendStatusSlot(QString)) );
    connect( &_playerLogger, SIGNAL(newPlayerConnected()), this, SLOT(newPlayerConnected()) );


    connect(&_view, SIGNAL(startClickedSignal()), this, SLOT(startRequestedSlot()) );
    connect(&_view, SIGNAL(exitSignal()), this, SLOT(quitSlot()) );

    //debug
    _view.appendStatus("Server Active; GameState set to NOPARTY, checker and logger started");
}

PongServer::~PongServer()
{
    for(int i=0; i < _playersStates.size(); ++i)
        _playersStates[i]->deleteLater();
}

void PongServer::start()
{
    _view.show();

    emit newGameSignal();
}

void PongServer::gameStateErrorSlot(const QString &mess)
{
    //when not enough players (only 1)
    _gameState.setStateError();

    qDebug() << mess << endl;
}

void PongServer::newPlayerConnected()
{
    qint32 nbPlayers;

    //debug
    _view.appendStatus("PongServer::newPlayersConnected : signal received");

    nbPlayers = _gameState.nbPlayers();

    if(nbPlayers > 1)
        _view.enableStartButton();
}

void PongServer::startRequestedSlot()
{

    _playingArea.rebuild( _gameState.nbPlayers() );
    _gameState.setStartRequested();

    //debug
    _view.appendStatus("PongServer::startRequestedSlot: gameState set to START_REQUESTED");
}

void PongServer::quitSlot()
{
    //debug
    _view.appendStatus("PongServer::quitSlot: quitSignal received");
    _view.disableStartButton();

    _gameState.setExitRequested();
}

void PongServer::threadTerminated()
{
    _view.appendStatus("a thread has terminated");

    if( _all_threads_finished() )
    {
        _view.appendStatus("All thread have terminated");
        _view.disableQuitButton();
    }
}

void PongServer::newGameSlot()
{
    //debug
    _view.appendStatus("PongServer::newGameSlot : resetting Game");

    _view.disableStartButton();

    //delete workers and thread for disconnected players

    if(_playersStates.size() > 0)
    {
        int i=0;
        while( i < _playersStates.size() )
        {
            _playersStates[i]->setId(i);

            if( _playersStates[i]->state() == PongTypes::DISCONNECTED )
            {
                _playersStates.erase( _playersStates.begin()+i );
                _socketWorkers.erase( _socketWorkers.begin()+i );
            }

            else
                ++i;
        }
    }

    if( _socketWorkers.size() > 1 )
        _view.enableStartButton();

    //reset gameState, playersStates
    _reset_gameState();
    _reset_playersStates();

    //start workers
    emit startService();

    //debug
    _view.appendStatus("Server Active; GameState set to NOPARTY; startService signal sent");
}

void PongServer::_reset_gameState()
{
    _gameState.setWaitingServer();
    _gameState.setNbPlayers( _socketWorkers.size() );
    _gameState.setLoserIndex(-1);
    _gameState.setDownCounter(0);
}

void PongServer::_reset_playersStates()
{
    for(int i=0; i < _playersStates.size(); ++i)
    {
        _playersStates[i]->setState(PongTypes::ACCEPTED);
        _playersStates[i]->setCredit( PlayerState::DefaultCredit() );
        _playersStates[i]->setdxRacket(0);
    }
}

bool PongServer::_all_threads_finished()
{
    bool finished;

    finished = _gameStateCheckerThread.isFinished();

    return finished;
}
