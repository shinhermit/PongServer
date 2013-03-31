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

void LoggerWorker::reset(const short & nbAcceptable)
{
    _nbAccepted = _maxPlayers - nbAcceptable;
}

void LoggerWorker::reset()
{
    _nbAccepted = 0;
}

void LoggerWorker::waitConnections()
{
    _tcpServer.listen(QHostAddress::Any, _port);
}

void LoggerWorker::newConnectionSlot()
{
    _gameState.lock();
    if(_nbAccepted < _maxPlayers && _gameState.state() != PongTypes::RUNNING)
    {
        _gameState.unlock();

        _playersStatesMutex.lock();
        _playersStates.push_back( new PlayerState(*this,PongTypes::ACCEPTED) );
        _playersStatesMutex.unlock();


        _sockets.push_back( _tcpServer.nextPendingConnection() );
        QTcpSocket & socket = *_sockets.at(_sockets.size()-1);

        _playersInterfaces.push_back( new SocketWorker(*this, socket, _playingArea, _gameState, _playersStates) );
        SocketWorker & interface = *_playersInterfaces[_playersInterfaces.size()-1];

        _playersInterfacesThreads.push_back(new QThread(this));
        QThread & interfaceThread = *_playersInterfacesThreads[_playersInterfacesThreads.size()-1];

        QObject::connect(&interfaceThread, SIGNAL(started()), &interface, SLOT(beginInteract()));
        interface.moveToThread(&interfaceThread);

        ++_nbAccepted;
    }
}
