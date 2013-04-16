#ifndef _GameStateWorker
#define _GameStateWorker

#include <cassert>

#include <QObject>
#include <QList>
#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QLineF>
#include <QPointF>
#include <QVector>
#include <QTimer>

#include "PlayingArea.hpp"
#include "PlayerState.hpp"
#include "GameState.hpp"
#include "PlayersStatesHolder.hpp"
#include "PongTypes.hpp"
#include "PongServerView.hpp"

class GameStateWorker : public QObject
{
    Q_OBJECT

public:
    GameStateWorker(
            GameState * globalGameState,
            PlayingArea * globalPlayingArea,
            PlayersStatesHolder * globalPlayersStates
            );

signals:
    void checkInitSignal();
    void checkRunningSignal();
    void beginMovingBallSignal();
    void finishedSignal();
    void appendStatusSignal(const QString & status);
    void gameOverSignal();

public slots:
    void waitStartSlot();
    void checkInitSlot();
    void checkRunningSlot();

private slots:
    void _countDownSlot();

private:
    QTimer _timer;
    qint32 _downCounter;

    PlayingArea _playingArea;
    QVector<PlayerState*> _playersStates;
    GameState _gameState;

    bool _exit_requested();

    void _update_rackets();

    void _check_collisions();

    void _manage_goal(const int & cageIndex);

    void _discard_player(const int & racketIndex);

    void _manage_wall_collision(const int & wallIndex);

    void _manage_racket_collision(const int & racketIndex);

    bool _game_over();

    void _manage_game_over();
};

#endif
