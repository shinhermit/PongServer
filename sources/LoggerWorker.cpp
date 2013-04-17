#include "LoggerWorker.hpp"

const short LoggerWorker::_maxPending = 12;

LoggerWorker::LoggerWorker(const qint16 &port):
    _port(port)
{
    _tcpServer.setMaxPendingConnections(_maxPending);
    connect( &_tcpServer, SIGNAL(newConnection()), this, SLOT(newConnectionSlot()) );
}

LoggerWorker::~LoggerWorker()
{
    if( _tcpServer.isListening() )
        _tcpServer.close();
}

void LoggerWorker::setlisteningPort(const qint16 &port)
{
    _port = port;
}

void LoggerWorker::waitConnections()
{
    if( !_exit_requested() )
    {
        if( !_tcpServer.isListening() )
            _tcpServer.listen(QHostAddress::Any, _port);

        //debug
        emit appendStatusSignal("LoggerWorker::waitConnections: listening");

    }

    else
        _tcpServer.close();
}

void LoggerWorker::newConnectionSlot()
{
    int index;

    //debug
    emit appendStatusSignal("LoggerWorker::newConnectionSlot: connection demand");

    if( !_loggableGameState() )
        emit appendStatusSignal("LoggerWorker::newConnectionSlot: unloggable gameState");

    if( _nbPlayers() <= PongShared::maxPlayers && _loggableGameState() )
    {
        lockPlayersStates();

        index = PongShared::playersStates.size();
        PongShared::playersStates.push_back( PlayerState(index, PongTypes::ACCEPTED, this) );

        QTcpSocket * socket = _tcpServer.nextPendingConnection();
        SocketWorker * worker = new SocketWorker(socket, index, this);

        connect( socket, SIGNAL(disconnected()), worker, SLOT(deleteLater()) );
        connect( worker, SIGNAL(appendStatusSignal(QString)), this, SLOT(appendStatusSlot(QString)) );
        connect( this, SIGNAL(startService()), worker, SLOT(beginInteract()) );

        _incNbPlayers();

        emit startService();
        emit newPlayerConnected();

        unlockPlayersStates();

        //debug
        emit appendStatusSignal("LoggerWorker::newConnectionSlot: connection accepted");
        if( _tcpServer.isListening() )
            emit appendStatusSignal("LoggerWorker::newConnectionSlot: Still listenning");
        else
            emit appendStatusSignal("LoggerWorker::newConnectionSlot: no more listening");
    }

    else if( _exit_requested() )
        _tcpServer.close();
}

void LoggerWorker::appendStatusSlot(const QString &status)
{
    emit appendStatusSignal(status);
}

bool LoggerWorker::_loggableGameState()
{
    bool loggable;

    lockGameState();
    loggable =
            (PongShared::gameState.state() != PongTypes::INITIALIZING
            &&
            PongShared::gameState.state() != PongTypes::RUNNING
            &&
            PongShared::gameState.state() != PongTypes::PAUSED
            &&
            PongShared::gameState.state() != PongTypes::EXIT_REQUESTED);
    unlockGameState();

    return loggable;
}

bool LoggerWorker::_exit_requested()
{
    bool requested;

    lockGameState();
    requested = ( PongShared::gameState.state() == PongTypes::EXIT_REQUESTED );
    unlockGameState();

    return requested;
}

void LoggerWorker::_setNbPlayers(const qint32 &nbPlayers)
{
    lockGameState();
    PongShared::gameState.setNbPlayers(nbPlayers);
    unlockGameState();
}

void LoggerWorker::_incNbPlayers()
{
    lockGameState();
    PongShared::gameState.setNbPlayers( PongShared::gameState.nbPlayers() + 1 );
    unlockGameState();
}

qint32 LoggerWorker::_nbPlayers()
{
    qint32 nbPlayers;

    lockGameState();
    nbPlayers = PongShared::gameState.nbPlayers();
    unlockGameState();

    return nbPlayers;
}
