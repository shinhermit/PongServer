#include "LobbyAgent.hpp"

LobbyAgent::LobbyAgent(QTcpSocket *socket):
    _socket(socket),
    _nbPlayersTowait(0),
    _startSignalSent(false)
{
    connect( _socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(_socketError(QAbstractSocket::SocketError)) );
    connect( _socket, SIGNAL(readyRead()), this, SLOT(ordersReceived()) );

    connect( this, SIGNAL(beginCheckNbPlayers()), this, SLOT(checkNbPlayers()) );
}

LobbyAgent::~LobbyAgent()
{
    if( _socket->isOpen() )
        _socket->close();

    _socket->deleteLater();
}

void LobbyAgent::connectToLobby(const QString & host, const qint16 &port)
{
    if( !_exit_requested() )
        _socket->connectToHost( host, port );

    else
        _finish();
}

void LobbyAgent::operator<<(QDataStream & in)
{
    QString nbPlayers;

    if( !_exit_requested() )
    {
        in >> nbPlayers;
        qDebug() << "LobbyAgent::operator<<: nbPlayers = " << nbPlayers;

        if( nbPlayers != "" && nbPlayers.toInt() > 0 )
            _nbPlayersTowait = nbPlayers.toInt();
    }

    else
        _finish();
}

void LobbyAgent::ordersReceived()
{
    qDebug() << "LobbyAgent::ordersReceived : available = " << _socket->bytesAvailable();
    QByteArray raw;
    QDataStream rawStream(raw);
    QDataStream socketStream(_socket);

    if( !_exit_requested() )
    {
        socketStream >> raw;
        (*this) << rawStream;

        emit beginCheckNbPlayers();
    }

    else
        _finish();

}

void LobbyAgent::checkNbPlayers()
{
    _startSignalSent = false;

    if( !_exit_requested() )
        QTimer::singleShot( 5, this, SLOT(_check_NbPlayers_routine()) );

    else
        _finish();
}

void LobbyAgent::_check_NbPlayers_routine()
{
    bool exitRequested = _exit_requested();

    if( _nbPlayers() == _nbPlayersTowait && !exitRequested )
    {
        _startSignalSent = true;
        emit startSignal();
    }

    else if( !exitRequested )
        QTimer::singleShot( 500, this, SLOT(_check_NbPlayers_routine()) );

    else
        _finish();
}

void LobbyAgent::_socketError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "LobbyAgent::_socketError: error code = " << QString::number(socketError) << endl;

    if( _socket->isOpen() )
    {
        if( _socket->state() == QAbstractSocket::ConnectedState )
            _socket->disconnectFromHost();
        _socket->close();
    }

    emit appendStatus("LobbyAgent::_socketError : an error occured");

    if( _exit_requested() )
        _finish();
}

qint32 LobbyAgent::_nbPlayers()
{
    qint32 nbPlayers;

    lockGameState();
    nbPlayers = PongShared::gameState.nbPlayers();
    unlockGameState();

    return nbPlayers;
}
