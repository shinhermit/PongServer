#ifndef _PongServer
#define _PongServer

#include <QWidget>
#include <QThread>
#include <QVector>
#include <QMutex>
#include <QDebug>

#include "Workers.hpp"
#include "GameState.hpp"
#include "PlayingArea.hpp"
#include "PlayerState.hpp"
#include "PongServerView.hpp"

class PongServer : public QObject
{
    Q_OBJECT
public:
    PongServer(const int & maxPlayers=6,
               const int & renderAreaWidth=600,
               const qint16 & port=6666);

    ~PongServer();

    void start();

signals:
    void gameStateErrorSignal(const QString & mess);
    void newGameSignal();
    void startService();

public slots:
    void newGameSlot();
    void gameStateErrorSlot(const QString & mess);
    void newPlayerConnected(SocketWorker*worker, QThread*thread);
    void startRequestedSlot();
    void quitSlot();
    void threadTerminated();

private:
    short _maxPlayers;
    PongServerView _view;

    //shared memories
    GameState _gameState; //shared memory
    PlayingArea _playingArea; //shared memory
    QVector<PlayerState*> _playersStates; //shared memory

    //Mutex for shared memory
    QMutex _playersStatesMutex;

    //worker classes for threads
    GameStateWorker _gameStateChecker;
    LoggerWorker _playerLogger;
    QVector<SocketWorker*> _socketWorkers;

    //thread objects
    QThread _gameStateCheckerThread;
    QThread _playerLoggerThread;
    QVector<QThread*> _socketThreads;

    void _reset_gameState();
    void _reset_playersStates();
    bool _all_threads_finished();
};

#endif
