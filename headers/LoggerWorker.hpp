#ifndef _LoggerWorker
#define _LoggerWorker

#include <cmath>

#include <QObject>
#include <QThread>
#include <QVector>
#include <QMutex>
#include <QTcpServer>
#include <QTcpSocket>

#include "PlayerState.hpp"
#include "GameState.hpp"
#include "PlayingArea.hpp"
#include "SocketWorker.hpp"
#include "PongServerView.hpp"

class LoggerWorker : public QObject
{
    Q_OBJECT
public:
    LoggerWorker(
            PongServerView & view,
            QTcpServer & tcpServer,
            QVector<QTcpSocket*> & sockets,
            PlayingArea & playingArea,
            GameState & gameState,
            QVector<PlayerState*> & playersStates,
            QMutex & playersStatesMutex,
            QVector<SocketWorker*> & playersInterfaces,
            QVector<QThread*> & playersInterfacesThreads,
            const qint16 & port=6666
            );

    void setNbConnected(const qint32 & nbConnected);
    qint32 nbConnected()const;

signals:
    void newPlayersConnected();

public slots:
    void waitConnections();
    void newConnectionSlot();

private:
    short _nbConnected;
    const qint16 _port;

    PongServerView & _view;
    QTcpServer & _tcpServer;
    QVector<QTcpSocket*> & _sockets;

    PlayingArea & _playingArea;
    GameState & _gameState;
    QVector<PlayerState*> & _playersStates;
    QMutex & _playersStatesMutex;
    QVector<SocketWorker*> & _playersInterfaces;
    QVector<QThread*> & _playersInterfacesThreads;

    static const short _maxPlayers;
    static const short _maxPending;

    bool _loggableGameState();
};

#endif
