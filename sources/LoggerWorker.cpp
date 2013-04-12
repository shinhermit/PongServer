#include "LoggerWorker.hpp"

const short LoggerWorker::_maxPlayers = 6;
const short LoggerWorker::_maxPending = 12;

LoggerWorker::LoggerWorker(
        PongServerView &view,
        QTcpServer &tcpServer,
        QVector<QTcpSocket *> &sockets,
        PlayingArea &playingArea,
        GameState &gameState,
        QVector<PlayerState *> &playersStates,
        QMutex &playersStatesMutex,
        QVector<SocketWorker *> &playersInterfaces,
        QVector<QThread *> &playersInterfacesThreads, const qint16 &port
        ):
    _nbConnected(0),
    _port(port),
    _view(view),
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

void LoggerWorker::setNbConnected(const qint32 &nbConnected)
{
    _nbConnected = ::abs(nbConnected);
}

qint32 LoggerWorker::nbConnected() const
{
    return _nbConnected;
}

void LoggerWorker::waitConnections()
{
    if( !_tcpServer.isListening() )
        _tcpServer.listen(QHostAddress::Any, _port);

    //debug
    _view.lock();
    _view.appendStatus("LoggerWorker::waitConnections: listening");
    _view.unlock();
}

void LoggerWorker::newConnectionSlot()
{
    int index;

    //debug
    _view.lock();
    _view.appendStatus("LoggerWorker::newConnectionSlot: connection demand");
    _view.unlock();

    if( _nbConnected <= _maxPlayers && _loggableGameState() )
    {
        _playersStatesMutex.lock();
        _playersStates.push_back( new PlayerState(PongTypes::ACCEPTED) );

        index = _sockets.size();
        _sockets.push_back( _tcpServer.nextPendingConnection() );
        QTcpSocket & socket = *_sockets.at(index);

        index = _playersInterfaces.size();
        _playersInterfaces.push_back( new SocketWorker(index, _view, socket, _playingArea, _gameState, _playersStates) );
        SocketWorker & interface = *_playersInterfaces[index];

        index = _playersInterfacesThreads.size();
        _playersInterfacesThreads.push_back(new QThread());
        QThread & interfaceThread = *_playersInterfacesThreads[index];

        connect(&interfaceThread, SIGNAL(started()), &interface, SLOT(beginInteract()));
        connect( &interface, SIGNAL(hostDisconnected()), &interfaceThread, SLOT(quit()) );
        connect( this, SIGNAL(quitSignal()), &interface, SLOT(quitSlot()) );
        connect( &interface, SIGNAL(finishedSignal()), &interfaceThread, SLOT(quit()) );

        interface.moveToThread(&interfaceThread);
        interfaceThread.start();

        ++_nbConnected;

        _gameState.lock();
        _gameState.setNbPlayers(_nbConnected);
        _gameState.unlock();

        emit newPlayersConnected();

        _playersStatesMutex.unlock();

        //debug
        _view.lock();
        _view.appendStatus("LoggerWorker::newConnectionSlot: connection accepted");
        _view.unlock();
    }
}

void LoggerWorker::quitSlot()
{
    _tcpServer.close();
    emit quitSignal();
    emit finishedSignal();
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
