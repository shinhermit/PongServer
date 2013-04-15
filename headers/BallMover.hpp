#ifndef BALLMOVER_HPP
#define BALLMOVER_HPP

#include <cmath>

#include <QObject>
#include <QTimer>

#include "PlayingArea.hpp"
#include "GameState.hpp"
#include "PongTypes.hpp"

class BallMover : public QObject
{
    Q_OBJECT
public:
    BallMover(
            GameState * gameState,
            PlayingArea *playingArea,
            const int & period=100
            );

    ~BallMover();

    const int & period()const;
    void setPeriod(const int & period);

signals:
    void finishedSignal();

public slots:
    void beginMovingBall();

private slots:
    void _moveBall();

private:
    QTimer _timer;
    int _period;

    GameState * _gameState;
    PlayingArea * _playingArea;

    bool _running_state()const;
};

#endif // BALLMOVER_HPP
