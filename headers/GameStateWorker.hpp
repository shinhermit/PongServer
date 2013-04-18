#ifndef _GameStateWorker
#define _GameStateWorker

#include <cassert>

#include <QObject>
#include <QVector>
#include <QTimer>

#include "Concurrent.hpp"
#include "PongShared.hpp"
#include "PongTypes.hpp"
#include "PongServerView.hpp"

class GameStateWorker : public QObject, public Concurrent
{
    Q_OBJECT

public:
    GameStateWorker(QObject * parent=0);

signals:
    void checkInitSignal();
    void checkRunningSignal();
    void finishedSignal();
    void appendStatusSignal(const QString & status);
    void gameOverSignal();
    void startMovingBall();
    void stopMovingBall();

public slots:
    void waitStartSlot();
    void checkInitSlot();
    void checkRunningSlot();

private slots:
    void _countDownSlot();

    void _check_running_routine();

private:
    QTimer _timer;
    qint32 _downCounter;

    bool _exit_requested();

    void _update_rackets();

    void _check_collisions();

    void _manage_goal(const int & cageIndex);

    void _discard_player(const int & racketIndex);

    void _manage_wall_collision(const int & wallIndex);

    void _manage_racket_collision(const int & racketIndex);

    bool _game_over();

    void _manage_game_over();

    void _move_ball();
};

#endif
