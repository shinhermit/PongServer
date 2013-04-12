#ifndef _PongServer
#define _PongServer

#include <QWidget>
#include <QThread>
#include <QVector>
#include <QMutex>
#include <QDebug>

#include <utility>

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
    void stopService();

public slots:
    void newGameSlot();
    void gameStateErrorSlot(const QString & mess);
    void newPlayersConnected();
    void startRequestedSlot();
    void quitSlot();

private:
    short _maxPlayers;
    PongServerView _view;

    //shared memories
    GameState _gameState; //shared memory
    PlayingArea _playingArea; //shared memory
    QVector<PlayerState*> _playersStates; //shared memory

    //Mutexes for shared memories
    QMutex _playersStatesMutex;

    //network
    QTcpServer _tcpServer;
    QVector<QTcpSocket*> _sockets;

    //worker classes for threads
    GameStateWorker _gameStateChecker;
    LoggerWorker _playerLogger;
    QVector<SocketWorker*> _playersInterfaces;

    //thread objects
    QThread _gameStateCheckerThread;
    QThread _playerLoggerThread;
    QVector<QThread*> _playersInterfacesThreads;

    void _reset_gameState();
    void _reset_playersStates();
    bool _exists_running_interface()const;
};

#endif
