#include "SocketWorker.hpp"

SocketWorker::SocketWorker(QTcpSocket * socket,
                           const qint32 & playerIndex,
                           QObject *parent):
    QObject(parent),
    _playerIndex(playerIndex),
    _socket(socket)
{
    connect( _socket, SIGNAL(readyRead()), this, SLOT(getDataSlot()) );
    connect( this, SIGNAL(sendDataSignal()), this, SLOT(sendDataSlot()) );
    connect( _socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)) );
    connect( _socket, SIGNAL(disconnected()), this, SLOT(disconnected()) );
}


SocketWorker::~SocketWorker()
{
    if( _socket->isOpen() )
        _socket->close();

    _socket->deleteLater();
}

void SocketWorker::operator>>(QDataStream &out)
{
    qint32 nbRackets, nbPlayers, loserIndex, gameState, downCounter=-1;
    QPointF ballPos;
    QLineF racketLine;


    lockGameState();
    loserIndex = PongShared::gameState.loserIndex();
    gameState = PongShared::gameState.state();
    nbPlayers = PongShared::gameState.nbPlayers();

    if( gameState == PongTypes::INITIALIZING )
        downCounter = PongShared::gameState.downCounter();
    unlockGameState();

    lockPlayingArea();
    nbRackets = PongShared::playingArea.nbRackets();
    ballPos = PongShared::playingArea.ballPos();

    out << ballPos << _playerIndex << nbRackets-1 << nbPlayers << loserIndex << gameState << downCounter;
    for(qint32 playerIndex=0;
        playerIndex<nbPlayers && playerIndex < nbRackets;
        ++playerIndex)
    {
        if(playerIndex != _playerIndex)
        {
            racketLine = PongShared::playingArea.racketLine(playerIndex);

            out << playerIndex << racketLine.p1() << racketLine.p2();
        }
    }
    unlockPlayingArea();

}

void SocketWorker::operator<<(QDataStream & in)
{
    QPointF point1, point2;
    qint32 id;

    in >> id >> point1 >> point2;

    QLineF racket(point1,point2);
    lockPlayersStates();
    if( _active_player() )
        PongShared::playersStates[_playerIndex].setRacket(racket);
    unlockPlayersStates();
}

QDataStream & operator<<(QDataStream &out, SocketWorker &sckw)
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
        if( _socket->isOpen() )
            _socket->close();
        //debug
        emit appendStatusSignal("SocketWorker::beginInteract: exit requested, can not enter interact routine");
    }
}

void SocketWorker::sendDataSlot()
{
    QByteArray raw;
    QDataStream socketStream(_socket);
    QDataStream rawStream(&raw, QIODevice::ReadWrite);

    if( !_exit_requested() )
    {
        //debug
        (*this) >> rawStream;
        socketStream << raw;
    }

    else
    {
        if( _socket->isOpen() )
            _socket->close();
        //debug
        emit appendStatusSignal("SocketWorker::sendDataSlot: exit requested, leaving interact routine");
    }
}

void SocketWorker::getDataSlot()
{
    QByteArray raw;
    QDataStream socketStream(_socket);
    QDataStream rawStream(&raw, QIODevice::ReadWrite);

    if( !_exit_requested() )
    {
        socketStream >> raw;
        (*this) << rawStream;
        emit sendDataSignal();
    }

    else
    {
        if( _socket->isOpen() )
            _socket->close();
        //debug
        emit appendStatusSignal("SocketWorker::getDataSlot: exit requested, leaving interact routine");
    }
}

void SocketWorker::socketError(QAbstractSocket::SocketError socketError)
{
    lockPlayersStates();
    PongShared::playersStates[_playerIndex].setState(PongTypes::SOCKET_ERROR);
    unlockPlayersStates();

    qDebug() << "SocketWorker::socketError: error code = " << QString::number(socketError) << endl;

    if( _socket->isOpen() )
        _socket->close();
    //debug
    emit appendStatusSignal( "SocketWorker::socketError: error code = " + QString::number(socketError) );
}

void SocketWorker::disconnected()
{
    //debug
    emit appendStatusSignal("SocketWorker::disconnected: players "+QString::number(_playerIndex)+" disconnected");

    lockPlayersStates();
    PongShared::playersStates[_playerIndex].setState(PongTypes::DISCONNECTED);
    PongShared::playersStates[_playerIndex].setRacket(500,500,500,500);
    unlockPlayersStates();

    lockGameState();
    PongShared::gameState.decNbPlayers();
    PongShared::gameState.decNbActive();
    unlockGameState();

    lockPlayingArea();
    PongShared::playingArea.removeCage(_playerIndex);
    PongShared::playingArea.setRacketLine(_playerIndex, QLineF(500,500,500,500));
    unlockPlayingArea();

    if( _socket->isOpen() )
        _socket->close();
    deleteLater();
}

bool SocketWorker::_running_state()
{
    PongTypes::E_GameState gameState;
    PongTypes::E_PlayerState playerState;

    lockGameState();
    gameState = PongShared::gameState.state();
    unlockGameState();

    lockPlayersStates();
    playerState = PongShared::playersStates[_playerIndex].state();
    unlockPlayersStates();

    return (gameState != PongTypes::GAMEOVER
            &&
            gameState != PongTypes::EXIT_REQUESTED
            &&
            playerState != PongTypes::SOCKET_ERROR
            &&
            playerState != PongTypes::DISCONNECTED);
}

bool SocketWorker::_active_player()
{
    bool active;

    lockPlayersStates();
    active = (PongShared::playersStates[_playerIndex].state() != PongTypes::DISCARDED
            &&
            PongShared::playersStates[_playerIndex].state() != PongTypes::DISCONNECTED
            );
    unlockPlayersStates();

    return active;
}

bool SocketWorker::_exit_requested()
{
    bool requested;

    lockGameState();
    requested = ( PongShared::gameState.state() == PongTypes::EXIT_REQUESTED );
    unlockGameState();

    return requested;
}
