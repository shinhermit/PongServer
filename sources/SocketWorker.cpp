#include "SocketWorker.hpp"

SocketWorker::SocketWorker(QTcpSocket * socket,
                           const qint32 & playerIndex,
                           QObject *parent):
    Concurrent(parent),
    _playerIndex(playerIndex),
    _socket(socket),
    _disconnected(false)
{
    connect( _socket, SIGNAL(readyRead()), this, SLOT(getDataSlot()) );
    connect( this, SIGNAL(sendDataSignal()), this, SLOT(sendDataSlot()) );
    connect( _socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)) );
    connect( _socket, SIGNAL(disconnected()), this, SLOT(disconnected()) );
}


SocketWorker::~SocketWorker()
{
    if( _socket && _socket->isOpen() )
        _socket->close();

    _socket->deleteLater();
}

void SocketWorker::operator>>(QDataStream &out)
{
    qint32 nbRackets, nbPlayers, loserIndex, gameState, downCounter=-1;
    bool discarded;
    QPointF ballPos;
    QLineF racketLine;

    if(!_disconnected)
    {
        lockGameState();
        loserIndex = PongShared::gameState.loserIndex();
        gameState = PongShared::gameState.state();
        nbPlayers = PongShared::gameState.nbPlayers();

        if( gameState == PongTypes::INITIALIZING )
            downCounter = PongShared::gameState.downCounter();
        unlockGameState();

        lockPlayersStates();
        if( _playerIndex < PongShared::playersStates.size() )
            discarded = PongShared::playersStates[_playerIndex].state() == PongTypes::DISCARDED;

        else
            qDebug() << "SocketWorker::operator>> : " << _playerIndex << " is out of bounds [0," << PongShared::playersStates.size()-1 << "]";
        unlockPlayersStates();

        lockPlayingArea();
        nbRackets = PongShared::playingArea.nbRackets();
        ballPos = PongShared::playingArea.ballPos();

        out << ballPos << _playerIndex << nbRackets-1 << nbPlayers << loserIndex << gameState << discarded << downCounter;
        for(qint32 playerIndex=0;
            playerIndex < nbPlayers && playerIndex < nbRackets;
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
}

void SocketWorker::operator<<(QDataStream & in)
{
    QPointF point1, point2;
    qint32 id;

    if(!_disconnected)
    {
        in >> id >> point1 >> point2;

        QLineF racket(point1,point2);
        lockPlayersStates();
        if( _active_player() )
        {
            if(_playerIndex < PongShared::playersStates.size())
                PongShared::playersStates[_playerIndex].setRacket(racket);

            else
                qDebug() << "SocketWorker::operator<< : index " << _playerIndex << " is out of bounds [0," << PongShared::playersStates.size()-1 << "]";
        }
        unlockPlayersStates();
    }
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

    if(!_disconnected)
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
}

void SocketWorker::sendDataSlot()
{
    QByteArray raw;
    QDataStream socketStream(_socket);
    QDataStream rawStream(&raw, QIODevice::ReadWrite);

    if(!_disconnected)
    {
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
}

void SocketWorker::getDataSlot()
{
    QByteArray raw;
    QDataStream socketStream(_socket);
    QDataStream rawStream(&raw, QIODevice::ReadWrite);

    if(!_disconnected)
    {
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
}

void SocketWorker::socketError(QAbstractSocket::SocketError socketError)
{
    if(!_disconnected)
    {
        lockPlayersStates();
        if( _playerIndex < PongShared::playersStates.size() )
            PongShared::playersStates[_playerIndex].setState(PongTypes::SOCKET_ERROR);

        else
            qDebug() << "SocketWorker::socketError : index " << _playerIndex << " is out of bounds [0," << PongShared::playersStates.size()-1<< "]";
        unlockPlayersStates();

        qDebug() << "SocketWorker::socketError: error code = " << QString::number(socketError) << endl;

        if( _socket->isOpen() )
            _socket->close();
        //debug
        emit appendStatusSignal( "SocketWorker::socketError: error code = " + QString::number(socketError) );
    }
}

void SocketWorker::disconnected()
{
    //debug
    emit appendStatusSignal("SocketWorker::disconnected: players "+QString::number(_playerIndex)+" disconnected");

    _disconnected = true;

    lockPlayersStates();
    if( _playerIndex < PongShared::playersStates.size() )
    {
        PongShared::playersStates[_playerIndex].setState(PongTypes::DISCONNECTED);
        PongShared::playersStates[_playerIndex].setRacket(500,500,500,500);
    }

    else
        qDebug() << "SocketWorker::disconnected: index " << _playerIndex << " is out of bounds [0," << PongShared::playersStates.size()-1<< "]";
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

    if(!_disconnected)
    {
        lockGameState();
        gameState = PongShared::gameState.state();
        unlockGameState();

        lockPlayersStates();
        if( _playerIndex < PongShared::playersStates.size() )
            playerState = PongShared::playersStates[_playerIndex].state();


        else
            qDebug() << "SocketWorker::_running_state: index " << _playerIndex << " is out of bounds [0," << PongShared::playersStates.size()-1<< "]";
        unlockPlayersStates();

        return (gameState != PongTypes::GAMEOVER
                &&
                gameState != PongTypes::EXIT_REQUESTED
                &&
                playerState != PongTypes::SOCKET_ERROR
                &&
                playerState != PongTypes::DISCONNECTED);
    }
}

bool SocketWorker::_active_player()
{
    bool active=false;

    if(!_disconnected)
    {
        lockPlayersStates();
        if( _playerIndex < PongShared::playersStates.size() )
            active = (PongShared::playersStates[_playerIndex].state() != PongTypes::DISCARDED
                    &&
                    PongShared::playersStates[_playerIndex].state() != PongTypes::DISCONNECTED
                    );

        else
            qDebug() << "SocketWorker::_active_players : index " << _playerIndex << " is out of bounds [0," << PongShared::playersStates.size()-1<< "]";
        unlockPlayersStates();
    }

    return active;
}
