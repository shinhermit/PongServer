#include "PongServer.hpp"

PongServer::PongServer(const int & maxPlayers,
                       const int & renderAreaWidth,
                       const qint16 & port)
  : _maxPlayers(maxPlayers),
    _playingArea(renderAreaWidth),
    _gameStateChecker(_playingArea, _playersStates, _playersStatesMutex, _gameState),
    _playerLogger(_tcpServer, _sockets, _playingArea, _gameState, _playersStates, _playersStatesMutex, _playersInterfaces, _playersInterfacesThreads, port)
{
    connect(this, SIGNAL(startService()), &_gameStateChecker, SLOT(waitStartSlot()) );

    connect(this, SIGNAL(startService()), &_playerLogger, SLOT(waitConnections()) );

    connect(&_playerLogger, SIGNAL(newPlayersConnected()), this, SLOT(newPlayersConnected()) );

    connect(&_view, SIGNAL(startClickedSignal()), this, SLOT(startRequestedSlot()) );

    _gameStateChecker.moveToThread(&_gameStateCheckerThread);
  _playerLogger.moveToThread(&_playerLoggerThread);

  _gameStateCheckerThread.start();
  _playerLoggerThread.start();
}

void PongServer::gameStateErrorSlot(const QString &mess)
{
    //when not enough players (only 1)
    _gameState.lock();
    _gameState.setStateError();
    _gameState.unlock();

    qDebug() << mess << endl;
}

void PongServer::newPlayersConnected()
{
    qint32 nbPlayers;

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
}

void PongServer::newGame()
{
    //delete workers and thread for disconnected players
    _playersStatesMutex.lock();
    if(_playersStates.size() > 0)
    {
        int i=0;
        while( i < _playersStates.size() )
        {
            if( _playersStates[i]->state() == PongTypes::DISCONNECTED )
            {
                _playersStates.erase( _playersStates.begin()+i );
                _sockets.erase( _sockets.begin()+i );
                _playersInterfaces.erase( _playersInterfaces.begin()+i );
                _playersInterfacesThreads.erase( _playersInterfacesThreads.begin()+i );
            }

            else
                ++i;
        }
    }
    _playersStatesMutex.unlock();

    //update _myIndex for remaining Players
    for(int i=0; i < _playersInterfaces.size(); ++i)
        _playersInterfaces[i]->setId(i);

    //tell LoggerWorker how many players are already present
    _playerLogger.setNbConnected( _playersInterfaces.size() );

    //reset gameState, playersStates
    _reset_gameState();
    _reset_playersStates();

    //start workers
    emit startService();
}

void PongServer::_reset_gameState()
{
    _gameState.lock();

    _gameState.setWaitingServer();
    _gameState.setNbPlayers( _playersInterfaces.size() );
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
