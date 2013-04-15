#include "LoggerWorker.hpp"

const short LoggerWorker::_maxPlayers = 6;
const short LoggerWorker::_maxPending = 12;

LoggerWorker::LoggerWorker(
        GameState &gameState,
        PlayingArea &playingArea,
        QVector<PlayerState *> &playersStates,
        QMutex &playersStatesMutex,
        QVector<SocketWorker *> &socketWorkers,
        const qint16 &port
        ):
    _port(port),
    _playingArea(playingArea),
    _gameState(gameState),
    _playersStates(playersStates),
    _playersStatesMutex(playersStatesMutex),
    _socketWorkers(socketWorkers)
{
    _tcpServer.setMaxPendingConnections(_maxPending);
    connect( &_tcpServer, SIGNAL(newConnection()), this, SLOT(newConnectionSlot()) );
}

LoggerWorker::~LoggerWorker()
{
    if( _tcpServer.isListening() )
        _tcpServer.close();
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

    if( _nbPlayers() <= _maxPlayers && _loggableGameState() )
    {
        index = _playersStates.size();
        _playersStates.push_back( new PlayerState(index, PongTypes::ACCEPTED, this) );

        QTcpSocket & socket = *_tcpServer.nextPendingConnection();
        _socketWorkers.push_back( new SocketWorker(socket, _playingArea, _gameState, *_playersStates[index]) );
        index = _socketWorkers.size() - 1;
        SocketWorker * worker = _socketWorkers[index];

        connect( worker, SIGNAL(appendStatusSignal(QString)), this, SLOT(appendStatusSlot(QString)) );

        _incNbPlayers();

        worker->beginInteract();

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

    loggable =
            (_gameState.state() != PongTypes::INITIALIZING
            &&
            _gameState.state() != PongTypes::RUNNING
            &&
            _gameState.state() != PongTypes::PAUSED
            &&
            _gameState.state() != PongTypes::EXIT_REQUESTED);

    return loggable;
}

bool LoggerWorker::_exit_requested()
{
    bool requested;

    requested = ( _gameState.state() == PongTypes::EXIT_REQUESTED );

    return requested;
}

void LoggerWorker::_setNbPlayers(const qint32 &nbPlayers)
{
    _gameState.setNbPlayers(nbPlayers);
}

void LoggerWorker::_incNbPlayers()
{
    _gameState.setNbPlayers( _gameState.nbPlayers() + 1 );

}

qint32 LoggerWorker::_nbPlayers()
{
    qint32 nbPlayers;

    nbPlayers = _gameState.nbPlayers();

    return nbPlayers;
}
