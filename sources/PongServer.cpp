#include "PongServer.hpp"

PongServer::PongServer(const int & maxPlayers,
                       const int & renderAreaWidth,
                       const qint16 & port):
    _maxPlayers(maxPlayers),
    _gameState(PongTypes::NOPARTY),
    _playingArea(renderAreaWidth),
    _gameStateChecker(_gameState, _playingArea, _playersStates, _playersStatesMutex),
    _playerLogger(_gameState, _playingArea, _playersStates, _playersStatesMutex, _socketWorkers, _socketThreads, port)
{
    connect( this, SIGNAL(newGameSignal()), this, SLOT(newGameSlot()) );

    connect(&_gameStateCheckerThread, SIGNAL(started()), &_gameStateChecker, SLOT(waitStartSlot()) );
    connect( &_gameStateChecker, SIGNAL(finishedSignal()), &_gameStateCheckerThread, SLOT(quit()) );
    connect( &_gameStateChecker, SIGNAL(appendStatusSignal(QString)), &_view, SLOT(appendStatusSlot(QString)) );

    connect(&_playerLoggerThread, SIGNAL(started()), &_playerLogger, SLOT(waitConnections()) );
    connect( &_playerLogger, SIGNAL(finishedSignal()), &_playerLoggerThread, SLOT(quit()) );
    connect( &_playerLogger, SIGNAL(appendStatusSignal(QString)), &_view, SLOT(appendStatusSlot(QString)) );

    _gameStateChecker.moveToThread(&_gameStateCheckerThread);
    _playerLogger.moveToThread(&_playerLoggerThread);

    connect(&_playerLogger, SIGNAL(newPlayerConnected(SocketWorker*, QThread*)), this, SLOT(newPlayerConnected(SocketWorker*, QThread*)) );

    connect(&_view, SIGNAL(startClickedSignal()), this, SLOT(startRequestedSlot()) );
    connect(&_view, SIGNAL(exitSignal()), this, SLOT(quitSlot()) );

    //debug
    _view.appendStatus("Server Active; GameState set to NOPARTY");
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

void PongServer::newPlayerConnected(SocketWorker *worker, QThread *thread)
{
    qint32 nbPlayers;

    //debug
    _view.appendStatus("PongServer::newPlayersConnected : signal received");

    connect(worker, SIGNAL(appendStatusSignal(QString)), &_view, SLOT(appendStatusSlot(QString)) );
    connect(thread, SIGNAL(started()), worker, SLOT(beginInteract()));
    connect( worker, SIGNAL(hostDisconnected()), thread, SLOT(quit()) );
    connect( worker, SIGNAL(finishedSignal()), thread, SLOT(quit()) );

    worker->moveToThread(thread);
    thread->start();

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
    _view.appendStatus("PongServer::startRequestedSlot: gameState set to START_REQUESTED");
}

void PongServer::quitSlot()
{
    //debug
    _view.appendStatus("PongServer::quitSlot: quitSignal received");

    _gameState.lock();
    _gameState.setExitRequested();
    _gameState.unlock();

    //attendre la fin des threads

    _view.close();
}

void PongServer::newGameSlot()
{
    //debug
    _view.appendStatus("PongServer::newGameSlot : resetting Game");

    _view.disableStartButton();

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
        _view.enableStartButton();

    //reset gameState, playersStates
    _reset_gameState();
    _reset_playersStates();

    //start workers
    _gameStateCheckerThread.start();
    _playerLoggerThread.start();

    //debug
    _view.appendStatus("Server Active; GameState set to NOPARTY; Checker and Logger Threads lunched");
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

bool PongServer::_all_threads_finished()
{
    bool finished;
    int i;

    finished = false;

    finished = _playerLoggerThread.isFinished() && _gameStateCheckerThread.isFinished();

    if(finished)
    {
        i=0;
        while( i < _socketThreads.size() && finished)
        {
            finished = _socketThreads[i]->isFinished();
            ++i;
        }
    }

    return finished;
}
