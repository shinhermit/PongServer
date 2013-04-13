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
            QVector<QThread*> & socketThreads,
            const qint16 & port=6666
            );

    ~LoggerWorker();

signals:
    void newPlayerConnected(SocketWorker * worker, QThread * thread);
    void finishedSignal();
    void appendStatusSignal(const QString & status);

public slots:
    void waitConnections();
    void newConnectionSlot();

private slots:
    void _checkExitRequested();

private:
    const qint16 _port;
    QTcpServer _tcpServer;
    QTimer _timer;

    PlayingArea & _playingArea;
    GameState & _gameState;
    QVector<PlayerState*> & _playersStates;
    QMutex & _playersStatesMutex;
    QVector<SocketWorker*> & _socketWorkers;
    QVector<QThread*> & _socketThreads;

    static const int _timerInterval;
    static const short _maxPlayers;
    static const short _maxPending;

    bool _loggableGameState();

    bool _exit_requested();

    void _setNbPlayers(const qint32 & nbPlayers);

    void _incNbPlayers();

    qint32 _nbPlayers();
};

#endif
