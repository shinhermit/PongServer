#ifndef LOBBYAGENT_HPP
#define LOBBYAGENT_HPP

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QDebug>
#include <QDataStream>
#include <QByteArray>
#include <QTimer>

#include "PongShared.hpp"
#include "Concurrent.hpp"

class LobbyAgent : public Concurrent
{
    Q_OBJECT
public:
    LobbyAgent(QTcpSocket * socket);

    ~LobbyAgent();

    void connectToLobby(const QString & host="127.0.0.1", const qint16 & port=6666);

    void operator<<(QDataStream & in);

signals:
    void startSignal();
    void appendStatus(QString status);
    void finishedSignal();
    void beginCheckNbPlayers();

public slots:
    void ordersReceived();
    void checkNbPlayers();

private slots:
    void _socketError(QAbstractSocket::SocketError socketError);
    void _check_NbPlayers_routine();
    qint32 _nbPlayers();

private:
    QTcpSocket * _socket;
    qint32 _nbPlayersTowait;
    bool _startSignalSent;
};

#endif // LOBBYAGENT_HPP
