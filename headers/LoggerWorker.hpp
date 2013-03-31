#ifndef _LoggerWorker
#define _LoggerWorker

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

class LoggerWorker : public QObject
{
    Q_OBJECT
public:
    LoggerWorker(
            QTcpServer & tcpServer,
            QVector<QTcpSocket*> & sockets,
            PlayingArea & playingArea,
            GameState & gameState,
            QVector<PlayerState*> & playersStates,
            QMutex & playersStatesMutex,
            QVector<SocketWorker*> & playersInterfaces,
            QVector<QThread*> & playersInterfacesThreads,
            const qint16 & port=7777
            );

    void reset(const short &nbAcceptable);

    void reset();
signals:
    void _playerLoggedSignal(); //active le bouton "begin"

public slots:
    void waitConnections();
    void newConnectionSlot();

private:
    short _nbAccepted;
    const qint16 _port;

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
