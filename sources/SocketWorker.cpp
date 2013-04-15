#include "SocketWorker.hpp"

SocketWorker::SocketWorker(
        QTcpSocket &socket,
        PlayingArea &playingArea,
        GameState &gameState,
        PlayerState &playerState
        ):
    _streamer(&socket),
    _socket(socket),
    _playingArea(playingArea),
    _gameState(gameState),
    _playerState(playerState)
{
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

    nbRackets = _playingArea.nbRackets();

    loserIndex = _gameState.loserIndex();
    gameState = _gameState.state();
    nbPlayers = _gameState.nbPlayers();

    if( gameState == PongTypes::INITIALIZING )
        downCounter = _gameState.downCounter();

    out << _playingArea.ballPos() << _playerState.id() << nbRackets << nbPlayers << loserIndex << gameState << downCounter;
    for(qint32 playerIndex=0; playerIndex<nbPlayers; ++playerIndex)
    {
        QGraphicsLineItem & racket = *_playingArea.racket(playerIndex);

        racketLine = racket.line();

        p1_racket = racket.mapToScene(racketLine.p1());
        p2_racket = racket.mapToScene(racketLine.p2());

        out << playerIndex << p1_racket << p2_racket;
    }

}

void SocketWorker::operator<<(QDataStream & in)
{
    qreal dxRacket;

    in >> dxRacket;

    _playerState.setdxRacket(dxRacket);
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
    }
}

void SocketWorker::sendDataSlot()
{
    _streamer.resetStatus();

    if( !_exit_requested() )
    {
        (*this) >> _streamer;
    }

    else
    {
        //debug
        emit appendStatusSignal("SocketWorker::sendDataSlot: exit requested, leaving interact routine");
    }
}

void SocketWorker::getDataSlot()
{
    _streamer.resetStatus();

    if( !_exit_requested() )
    {
        (*this) << _streamer;

        emit sendDataSignal();
    }

    else
    {
        //debug
        emit appendStatusSignal("SocketWorker::getDataSlot: exit requested, leaving interact routine");
    }
}

void SocketWorker::socketError(QAbstractSocket::SocketError socketError)
{
    _playerState.setState(PongTypes::SOCKET_ERROR);

    qDebug() << "SocketWorker::socketError: error code = " << QString::number(socketError) << endl;

    //debug
    emit appendStatusSignal( "SocketWorker::socketError: error code = " + QString::number(socketError) );
}

void SocketWorker::disconnected()
{
    //debug
    emit appendStatusSignal("SocketWorker::disconnected: players "+QString::number(_playerState.id())+" disconnected");

    _playerState.setState(PongTypes::DISCONNECTED);

    //debug
    emit appendStatusSignal("SocketWorker::disconnected: signal hostDisconnected emitted");
}

bool SocketWorker::_running_state()
{
    PongTypes::E_GameState gameState;
    PongTypes::E_PlayerState playerState;

    gameState = _gameState.state();

    playerState = _playerState.state();

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

    requested = ( _gameState.state() == PongTypes::EXIT_REQUESTED );

    return requested;
}
