#include "BallMover.hpp"

BallMover::BallMover(
        GameState *gameState,
        PlayingArea * playingArea,
        const int & period
        ):
    _period(period),
    _gameState(gameState),
    _playingArea(playingArea)
{
    connect(&_timer, SIGNAL(timeout()), this, SLOT(_moveBall()) );
}

BallMover::~BallMover()
{
    if(_timer.isActive())
        _timer.stop();

    emit finishedSignal();
}

const int & BallMover::period() const
{
    return _period;
}

void BallMover::setPeriod(const int &period)
{
    _timer.stop();
    _period = ::abs(period);
    _timer.start(_period);
}

void BallMover::beginMovingBall()
{
    if( _running_state() )
        _timer.start(_period);

    else
        emit finishedSignal();
}

void BallMover::_moveBall()
{
    if( _running_state() )
        _playingArea->moveBall();

    else
    {
        _timer.stop();
        emit finishedSignal();
    }
}

bool BallMover::_running_state() const
{
    return ( _gameState->state() == PongTypes::RUNNING );
}
