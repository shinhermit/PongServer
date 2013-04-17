#ifndef _PongServer
#define _PongServer

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QThread>
#include <QVector>
#include <QMutex>
#include <QDebug>

#include "Workers.hpp"
#include "Concurrent.hpp"
#include "PongShared.hpp"
#include "PongServerView.hpp"

class PongServer : public QObject, public Concurrent
{
    Q_OBJECT
public:
    PongServer(const qint16 & port=6666,
               const qint32 &renderAreaWidth=600);

    void start();

    void showScene();

signals:
    void gameStateErrorSignal(const QString & mess);
    void newGameSignal();
    void startService();
    void stopService();

public slots:
    void newGameSlot();
    void gameStateErrorSlot(const QString & mess);
    void newPlayerConnected();
    void startRequestedSlot();
    void quitSlot();
    void threadTerminated();

//shared stuffs

private:
    PongServerView _view;
    QGraphicsScene * _scene;
    QGraphicsView _areaView;


    //worker classes for threads
    GameStateWorker _gameStateChecker;
    LoggerWorker _playerLogger;
    BallMover _ballMover;

    //thread object
    QThread _gameStateCheckerThread;
    QThread _ballMoverThread;


    void _reset_gameState();
    void _reset_playersStates();
    bool _all_threads_finished();
};

#endif
