#include "PongServer.hpp"

PongServer::PongServer(const int & maxPlayers,
                       const int & renderAreaWidth,
                       const qint16 & port)
  : _maxPlayers(maxPlayers),
    _gameState(PongTypes::NOPARTY),
    _playingArea(renderAreaWidth),
    _gameStateChecker(_view, _gameState, _playingArea, _playersStates, _playersStatesMutex),
    _playerLogger(_view, _gameState, _playingArea, _playersStates, _playersStatesMutex, _socketWorkers, _socketThreads, port)
{
    connect( this, SIGNAL(newGameSignal()), this, SLOT(newGameSlot()) );

    connect(this, SIGNAL(startService()), &_gameStateChecker, SLOT(waitStartSlot()) );
    connect( &_gameStateChecker, SIGNAL(finishedSignal()), &_gameStateCheckerThread, SLOT(quit()) );

    connect(this, SIGNAL(startService()), &_playerLogger, SLOT(waitConnections()) );
    connect( &_playerLogger, SIGNAL(finishedSignal()), &_playerLoggerThread, SLOT(quit()) );

    connect(&_playerLogger, SIGNAL(newPlayerConnected()), this, SLOT(newPlayerConnected()) );

    connect(&_view, SIGNAL(startClickedSignal()), this, SLOT(startRequestedSlot()) );
    connect(&_view, SIGNAL(closeSignal()), this, SLOT(quitSlot()) );

    _gameStateChecker.moveToThread(&_gameStateCheckerThread);
    _playerLogger.moveToThread(&_playerLoggerThread);

    _gameStateCheckerThread.start();
    _playerLoggerThread.start();

    //debug
    _view.lock();
    _view.appendStatus("Server Active; GameState set to NOPARTY; Ckecker and Logger Threads lunched");
    _view.unlock();
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
    _gameState.lock();
    _gameState.setStateError();
    _gameState.unlock();

    qDebug() << mess << endl;
}

void PongServer::newPlayerConnected()
{
    qint32 nbPlayers;

    //debug
    _view.lock();
    _view.appendStatus("PongServer::newPlayersConnected : signal received");
    _view.unlock();

    _gameState.lock();
    nbPlayers = _gameState.nbPlayers();
    _gameState.unlock();

    if(nbPlayers > 1)
        _view.enableStartButton();
}

void PongServer::startRequestedSlot()
{
    _gameState.lock();
    _gameState.setStartRequested();
    _gameState.unlock();

    //debug
    _view.lock();
    _view.appendStatus("PongServer::startRequestedSlot: gameState set to START_REQUESTED");
    _view.unlock();
}

void PongServer::quitSlot()
{
    //debug
    _view.lock();
    _view.appendStatus("PongServer::quitSlot: quitSignal received");
    _view.unlock();

    _gameState.lock();
    _gameState.setExitRequested();
    _gameState.unlock();

    //attendre la fin des threads

    _view.close();
}

void PongServer::newGameSlot()
{
    _view.lock();
    _view.disableStartButton();
    //debug
    _view.appendStatus("PongServer::newGameSlot : resetting Game");
    _view.unlock();

    //delete workers and thread for disconnected players

    _playersStatesMutex.lock();
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
                _socketThreads.erase( _socketThreads.begin()+i );
            }

            else
                ++i;
        }
    }
    _playersStatesMutex.unlock();

    if( _socketWorkers.size() > 1 )
    {
        _view.lock();
        _view.enableStartButton();
        _view.unlock();
    }

    //reset gameState, playersStates
    _reset_gameState();
    _reset_playersStates();

    //start workers
    emit startService();

    //debug
    _view.lock();
    _view.appendStatus("PongServer::newGameSlot : startService() signal emitted");
    _view.unlock();
}

void PongServer::_reset_gameState()
{
    _gameState.lock();

    _gameState.setWaitingServer();
    _gameState.setNbPlayers( _socketWorkers.size() );
    _gameState.setLoserIndex(-1);
    _gameState.setDownCounter(0);

    _gameState.unlock();
}

void PongServer::_reset_playersStates()
{
    for(int i=0; i < _playersStates.size(); ++i)
    {
        _playersStates[i]->lock();

        _playersStates[i]->setState(PongTypes::ACCEPTED);
        _playersStates[i]->setCredit( PlayerState::DefaultCredit() );
        _playersStates[i]->setdxRacket(0);

        _playersStates[i]->unlock();
    }
}
