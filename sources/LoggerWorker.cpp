#include "LoggerWorker.hpp"

const int LoggerWorker::_timerInterval = 300;
const short LoggerWorker::_maxPlayers = 6;
const short LoggerWorker::_maxPending = 12;

LoggerWorker::LoggerWorker(
        GameState &gameState,
        PlayingArea &playingArea,
        QVector<PlayerState *> &playersStates,
        QMutex &playersStatesMutex,
        QVector<SocketWorker *> &socketWorkers,
        QVector<QThread *> &socketThreads,
        const qint16 &port
        ):
    _port(port),
    _playingArea(playingArea),
    _gameState(gameState),
    _playersStates(playersStates),
    _playersStatesMutex(playersStatesMutex),
    _socketWorkers(socketWorkers),
    _socketThreads(socketThreads)
{
    _tcpServer.setMaxPendingConnections(_maxPending);
    connect( &_tcpServer, SIGNAL(newConnection()), this, SLOT(newConnectionSlot()) );

    connect( &_timer, SIGNAL(timeout()), this, SLOT(_checkExitRequested()) );
}

LoggerWorker::~LoggerWorker()
{
    if(_timer.isActive())
        _timer.stop();

    if( _tcpServer.isListening() )
        _tcpServer.close();
}

void LoggerWorker::waitConnections()
{
    //we don't want to be interrupted
    if( _timer.isActive() )
        _timer.stop();

    if( !_exit_requested() )
    {
        if( !_tcpServer.isListening() )
            _tcpServer.listen(QHostAddress::Any, _port);

        //debug
        emit appendStatusSignal("LoggerWorker::waitConnections: listening");

        _timer.start(_timerInterval);
    }

    else
    {
        _tcpServer.close();

        emit finishedSignal();
    }
}

void LoggerWorker::newConnectionSlot()
{
    int index;

    //we don't want to be interrupted
    if( _timer.isActive() )
        _timer.stop();

    //debug
    emit appendStatusSignal("LoggerWorker::newConnectionSlot: connection demand");

    if( _nbPlayers() <= _maxPlayers && _loggableGameState() )
    {
        _playersStatesMutex.lock();
        index = _playersStates.size();
        _playersStates.push_back( new PlayerState(index, PongTypes::ACCEPTED, this) );

        QTcpSocket & socket = *_tcpServer.nextPendingConnection();
        _socketWorkers.push_back( new SocketWorker(socket, _playingArea, _gameState, *_playersStates[index]) );
        index = _socketWorkers.size() - 1;
        SocketWorker * worker = _socketWorkers[index];

        _socketThreads.push_back( new QThread(this) );
        index = _socketThreads.size() - 1;
        QThread * thread = _socketThreads[index];


        _incNbPlayers();

        emit newPlayerConnected(worker, thread);

        _playersStatesMutex.unlock();

        //debug
        emit appendStatusSignal("LoggerWorker::newConnectionSlot: connection accepted");

        _timer.start(_timerInterval);
    }

    else if( _exit_requested() )
    {
        _tcpServer.close();
        emit finishedSignal();
    }
}

void LoggerWorker::_checkExitRequested()
{
    _timer.stop();

    if( _exit_requested() )
    {
        _tcpServer.close();
        emit finishedSignal();
    }

    _timer.start(_timerInterval);
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
            _gameState.state() != PongTypes::PAUSED
            &&
            _gameState.state() != PongTypes::EXIT_REQUESTED);
    _gameState.unlock();

    return loggable;
}

bool LoggerWorker::_exit_requested()
{
    bool requested;

    _gameState.lock();
    requested = ( _gameState.state() == PongTypes::EXIT_REQUESTED );
    _gameState.unlock();

    return requested;
}

void LoggerWorker::_setNbPlayers(const qint32 &nbPlayers)
{
    _gameState.lock();
    _gameState.setNbPlayers(nbPlayers);
    _gameState.unlock();
}

void LoggerWorker::_incNbPlayers()
{
    _gameState.lock();
    _gameState.setNbPlayers( _gameState.nbPlayers() + 1 );
    _gameState.unlock();

}

qint32 LoggerWorker::_nbPlayers()
{
    qint32 nbPlayers;

    _gameState.lock();
    nbPlayers = _gameState.nbPlayers();
    _gameState.unlock();

    return nbPlayers;
}
