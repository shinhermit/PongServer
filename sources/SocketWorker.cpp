#include "SocketWorker.hpp"

SocketWorker::SocketWorker(
        QTcpSocket &socket,
        PlayingArea &playingArea,
        GameState &gameState,
        PlayerState &playerState
        ):
    _socket_stream(&socket),
    _socket(socket),
    _playingArea(playingArea),
    _gameState(gameState),
    _playerState(playerState)
{
    //_socket_stream.setDevice(&_socket);

    connect( &_socket, SIGNAL(readyRead()), this, SLOT(getDataSlot()) );
    connect( this, SIGNAL(sendDataSignal()), this, SLOT(sendDataSlot()) );
    connect( &_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)) );
    connect( &_socket, SIGNAL(disconnected()), this, SLOT(disconnected()) );
}


SocketWorker::~SocketWorker()
{
    if( _socket.isOpen() )
        _socket.close();
}

void SocketWorker::operator>>(QDataStream &out) const
{
    qint32 nbRackets, nbPlayers, loserIndex, gameState, downCounter=-1;
    QLineF racketLine;
    QPointF p1_racket, p2_racket;

    _playingArea.lock();
    nbRackets = _playingArea.nbRackets();
    _playingArea.unlock();

    _gameState.lock();
    loserIndex = _gameState.loserIndex();
    gameState = _gameState.state();
    nbPlayers = _gameState.nbPlayers();

    if( gameState == PongTypes::INITIALIZING )
        downCounter = _gameState.downCounter();
    _gameState.unlock();

    out << _playerState.id() << nbRackets << nbPlayers << loserIndex << gameState << downCounter;
    for(int playerIndex=0; playerIndex<nbPlayers; ++playerIndex)
    {
        _playingArea.lock();
        QGraphicsLineItem & racket = *_playingArea.racket(playerIndex);

        racketLine = racket.line();

        p1_racket = racket.mapToScene(racketLine.p1());
        p2_racket = racket.mapToScene(racketLine.p2());
        _playingArea.unlock();

        out << playerIndex << p1_racket << p2_racket;
    }

}

void SocketWorker::operator<<(QDataStream & in)
{
    qreal dxRacket;

    in >> dxRacket;

    _playerState.lock();
    _playerState.setdxRacket(dxRacket);
    _playerState.unlock();
}

QDataStream & operator<<(QDataStream &out, const SocketWorker &sckw)
{
    sckw>>out;

    return out;
}

QDataStream & operator>>(QDataStream &in, SocketWorker &sckw)
{
    sckw<<in;

    return in;
}

void SocketWorker::beginInteract()
{

    if( !_exit_requested() )
    {
        //debug
        emit appendStatusSignal("SocketWorker::beginInteract: Entering interact routine");

        emit sendDataSignal();
    }

    else
    {
        //debug
        emit appendStatusSignal("SocketWorker::beginInteract: exit requested, can not enter interact routine");

        emit finishedSignal();
    }
}

void SocketWorker::sendDataSlot()
{
    if( !_exit_requested() )
    {
        //debug
        emit appendStatusSignal("SocketWorker::sendDataSlot: next step = send into stream");
        (*this) >> _socket_stream;
        emit appendStatusSignal("SocketWorker::sendDataSlot: data sent");
    }

    else
    {
        //debug
        emit appendStatusSignal("SocketWorker::sendDataSlot: exit requested, leaving interact routine");

        emit finishedSignal();
    }
}

void SocketWorker::getDataSlot()
{
    if( !_exit_requested() )
    {
        emit appendStatusSignal("SocketWorker::getDataSlot: next step = receive from stream");
        (*this) << _socket_stream;
        emit appendStatusSignal("SocketWorker::getDataSlot: data received");

        emit appendStatusSignal("SocketWorker::getDataSlot: next step = emit sendDataSignal()");
        emit sendDataSignal();
    }

    else
    {
        //debug
        emit appendStatusSignal("SocketWorker::getDataSlot: exit requested, leaving interact routine");

        emit finishedSignal();
    }
}

void SocketWorker::socketError(QAbstractSocket::SocketError socketError)
{
    _playerState.lock();
    _playerState.setState(PongTypes::SOCKET_ERROR);
    _playerState.unlock();

    qDebug() << "SocketWorker::socketError: error code = " << QString::number(socketError) << endl;

    //debug
    emit appendStatusSignal( "SocketWorker::socketError: error code = " + QString::number(socketError) );
}

void SocketWorker::disconnected()
{
    //debug
    emit appendStatusSignal("SocketWorker::disconnected: players disconnected");

    _playerState.lock();
    _playerState.setState(PongTypes::DISCONNECTED);
    _playerState.unlock();

    qDebug() << "SocketWorker::disconnected: player " << _playerState.id() << " disconnected " << endl;

    emit hostDisconnected();

    //debug
    emit appendStatusSignal("SocketWorker::disconnected: signal hostDisconnected emitted");
}

bool SocketWorker::_running_state()
{
    PongTypes::E_GameState gameState;
    PongTypes::E_PlayerState playerState;

    _gameState.lock();
    gameState = _gameState.state();
    _gameState.unlock();

    _playerState.lock();
    playerState = _playerState.state();
    _playerState.unlock();

    return (gameState != PongTypes::GAMEOVER
            &&
            gameState != PongTypes::EXIT_REQUESTED
            &&
            playerState != PongTypes::SOCKET_ERROR
            &&
            playerState != PongTypes::DISCONNECTED);
}

bool SocketWorker::_exit_requested()
{
    bool requested;

    _gameState.lock();
    requested = ( _gameState.state() == PongTypes::EXIT_REQUESTED );
    _gameState.unlock();

    return requested;
}
