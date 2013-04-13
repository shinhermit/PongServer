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
               const int & renderAreaWidth=256,
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
    void newPlayerConnected();
    void startRequestedSlot();
    void quitSlot();

private:
    short _maxPlayers;
    PongServerView _view;

    //shared memories
    GameState _gameState; //shared memory
    PlayingArea _playingArea; //shared memory
    QVector<PlayerState*> _playersStates; //shared memory

    //Mutex for shared memory
    QMutex _playersStatesMutex;

    //network
    QVector<SocketWorker*> _socketWorkers;
    QVector<QThread*> _socketThreads;

    //worker classes for threads
    GameStateWorker _gameStateChecker;
    LoggerWorker _playerLogger;

    //thread objects
    QThread _gameStateCheckerThread;
    QThread _playerLoggerThread;

    void _reset_gameState();
    void _reset_playersStates();
};

#endif
