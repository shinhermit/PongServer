#include "LoggerWorker.hpp"

const short LoggerWorker::_maxPlayers = 6;
const short LoggerWorker::_maxPending = 12;

LoggerWorker::LoggerWorker(
        QTcpServer & tcpServer,
        QVector<QTcpSocket*> & sockets,
        PlayingArea & playingArea,
        GameState & gameState,
        QVector<PlayerState *> &playersStates,
        QMutex & playersStatesMutex,
        QVector<SocketWorker*> & playersInterfaces,
        QVector<QThread*> & playersInterfacesThreads,
        const qint16 & port
        ):
    _nbAccepted(0),
    _port(port),
    _tcpServer(tcpServer),
    _sockets(sockets),
    _playingArea(playingArea),
    _gameState(gameState),
    _playersStates(playersStates),
    _playersStatesMutex(playersStatesMutex),
    _playersInterfaces(playersInterfaces),
    _playersInterfacesThreads(playersInterfacesThreads)
{
    _tcpServer.setMaxPendingConnections(_maxPending);
    QObject::connect( &_tcpServer, SIGNAL(newConnection()), this, SLOT(newConnectionSlot()) );
}

void LoggerWorker::waitConnections()
{
    _tcpServer.listen(QHostAddress::Any, _port);
}

void LoggerWorker::newConnectionSlot()
{
    int index;

    if( _nbAccepted <= _maxPlayers && _loggableGameState() )
    {
        _playersStatesMutex.lock();
        _playersStates.push_back( new PlayerState(*this,PongTypes::ACCEPTED) );

        index = _sockets.size();
        _sockets.push_back( _tcpServer.nextPendingConnection() );
        QTcpSocket & socket = *_sockets.at(index);

        index = _playersInterfaces.size();
        _playersInterfaces.push_back( new SocketWorker(*this, index, socket, _playingArea, _gameState, _playersStates) );
        SocketWorker & interface = *_playersInterfaces[index];

        index = _playersInterfacesThreads.size();
        _playersInterfacesThreads.push_back(new QThread(this));
        QThread & interfaceThread = *_playersInterfacesThreads[index];

        QObject::connect(&interfaceThread, SIGNAL(started()), &interface, SLOT(beginInteract()));
        interface.moveToThread(&interfaceThread);

        ++_nbAccepted;
        _playersStatesMutex.unlock();
    }
}

bool LoggerWorker::_loggableGameState()
{
    bool loggable;

    _gameState.lock();
    loggable =
            (_gameState.state() != PongTypes::INITIALIZING
            &&
            _gameState.state() != PongTypes::RUNNING
            &&
            _gameState.state() != PongTypes::PAUSED);
    _gameState.unlock();

    return loggable;
}
