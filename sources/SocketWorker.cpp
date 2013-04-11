#include "SocketWorker.hpp"

SocketWorker::SocketWorker(
        QObject &parent,
        qint32 myIndex,
        QTcpSocket &socket,
        PlayingArea &playingArea,
        GameState &gameState,
        QVector<PlayerState *> &playersStates
        ):
    QObject(&parent),
    _socket(socket),
    _playingArea(playingArea),
    _gameState(gameState),
    _playersStates(playersStates),
    _socket_stream(&_socket),
    _myIndex(myIndex)
{
    QObject::connect( &_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)) );
}


SocketWorker::SocketWorker(qint32 myIndex,
                           QTcpSocket &socket,
                           PlayingArea &playingArea,
                           GameState &gameState,
                           QVector<PlayerState *> &playersStates
                           ):
    _socket(socket),
    _playingArea(playingArea),
    _gameState(gameState),
    _playersStates(playersStates),
    _socket_stream(&_socket),
    _myIndex(myIndex)
{
    QObject::connect( &_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)) );
    QObject::connect( &_socket, SIGNAL(disconnected()), this, SLOT(disconnected()) );
}

void SocketWorker::operator >>(QDataStream &out) const
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

    out << _myIndex << nbRackets << nbPlayers << loserIndex << gameState << downCounter;
    for(int playerIndex=0; playerIndex<nbRackets; ++playerIndex)
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

void SocketWorker::operator <<(QDataStream & in)
{
    qreal dxRacket;

    in >> dxRacket;

    _playersStates[_myIndex]->lock();
    _playersStates[_myIndex]->setdxRacket(dxRacket);
    _playersStates[_myIndex]->unlock();
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

void SocketWorker::setId(const qint32 & index)
{
    _myIndex = index;
}

const qint32 &SocketWorker::id() const
{
    return _myIndex;
}

void SocketWorker::beginInteract()
{

    while( _running_state() )
    {
        (*this) >> _socket_stream;

        (*this) << _socket_stream;
    }
}

void SocketWorker::socketError(QAbstractSocket::SocketError socketError)
{
    _playersStates[_myIndex]->lock();
    _playersStates[_myIndex]->setState(PongTypes::SOCKET_ERROR);
    _playersStates[_myIndex]->unlock();

    qDebug() << "SocketWorker::socketError: " << socketError << endl;

}

void SocketWorker::disconnected()
{
    _playersStates[_myIndex]->lock();
    _playersStates[_myIndex]->setState(PongTypes::DISCONNECTED);
    _playersStates[_myIndex]->unlock();

    qDebug() << "SocketWorker::disconnected: player " << _myIndex << " disconnected " << endl;

    emit hostDisconnected();
}

bool SocketWorker::_running_state()
{
    PongTypes::E_GameState gameState;
    PongTypes::E_PlayerState playerState;

    _gameState.lock();
    gameState = _gameState.state();
    _gameState.unlock();

    _playersStates[_myIndex]->lock();
    playerState = _playersStates[_myIndex]->state();
    _playersStates[_myIndex]->unlock();

    return (gameState != PongTypes::GAMEOVER
            &&
            gameState != PongTypes::EXITED
            &&
            playerState != PongTypes::SOCKET_ERROR
            &&
            playerState != PongTypes::DISCONNECTED);
}
