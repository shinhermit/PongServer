#ifndef _LoggerWorker
#define _LoggerWorker

#include <cmath>

#include <QObject>
#include <QThread>
#include <QVector>
#include <QMutex>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>

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
            GameState & gameState,
            PlayingArea & playingArea,
            QVector<PlayerState*> & playersStates,
            QMutex & playersStatesMutex,
            QVector<SocketWorker*> & socketWorkers,
            const qint16 & port=6666
            );

    ~LoggerWorker();

signals:
    void newPlayerConnected();
    void appendStatusSignal(const QString & status);
    void startService();

public slots:
    void waitConnections();
    void newConnectionSlot();
    void appendStatusSlot(const QString & status);

private:
    const qint16 _port;
    QTcpServer _tcpServer;

    PlayingArea & _playingArea;
    GameState & _gameState;
    QVector<PlayerState*> & _playersStates;
    QMutex & _playersStatesMutex;
    QVector<SocketWorker*> & _socketWorkers;

    static const short _maxPlayers;
    static const short _maxPending;

    bool _loggableGameState();

    bool _exit_requested();

    void _setNbPlayers(const qint32 & nbPlayers);

    void _incNbPlayers();

    qint32 _nbPlayers();
};

#endif
