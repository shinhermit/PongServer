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
    int index, nbPlayers;
    bool loggable;

    //debug
    emit appendStatusSignal("LoggerWorker::newConnectionSlot: connection demand");

    loggable = _loggableGameState();

    lockGameState();
    nbPlayers = PongShared::gameState.nbPlayers();
    unlockGameState();

    if(!loggable)
        emit appendStatusSignal("LoggerWorker::newConnectionSlot: unloggable gameState");

    if( nbPlayers < PongShared::maxPlayers && loggable )
    {
        lockPlayersStates();

        index = PongShared::playersStates.size();
        PongShared::playersStates.push_back( PlayerState(index, PongTypes::ACCEPTED, this) );

        QTcpSocket * socket = _tcpServer.nextPendingConnection();
        SocketWorker * worker = new SocketWorker(socket, index, this);

//        QThread * workerThread = new QThread(this);
//        worker->moveToThread(workerThread);
//        workerThread->start();

        connect( worker, SIGNAL(appendStatusSignal(QString)), this, SLOT(appendStatusSlot(QString)) );
        connect( this, SIGNAL(startService()), worker, SLOT(beginInteract()) );

        unlockPlayersStates();

        lockGameState();
        PongShared::gameState.incNbPlayers();
        PongShared::gameState.incNbActive();
        unlockGameState();

        emit startService();
        emit newPlayerConnected();

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
