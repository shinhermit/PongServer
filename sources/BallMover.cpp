#include "BallMover.hpp"

BallMover::BallMover(const qint32 & period):
    _period(period)
{
    connect( &_timer, SIGNAL(timeout()), this, SLOT(moveBall()) );
}

BallMover::~BallMover()
{
    if(_timer.isActive())
        _timer.stop();
}

void BallMover::setPeriod(const qint32 &period)
{
    _period = ::abs(period);
}

qint32 BallMover::period() const
{
    return _period;
}


void BallMover::startMoving()
{
    emit appendStatusSignal("Signal startMovingBall received");

    if( _timer.isActive() )
        _timer.stop();

    if( !_exit_requested() )
        _timer.start(_period);

    else
        emit finishedSignal();
}

void BallMover::moveBall()
{
    if( !_exit_requested() )
    {
        lockPlayingArea();
        PongShared::playingArea.moveBall();
        unlockPlayingArea();
    }

    else
    {
        _timer.stop();
        emit finishedSignal();
    }
}

void BallMover::stopMoving()
{
    emit appendStatusSignal("BallMover::stopMoving : Signal stopMovingBall received");

    if(_timer.isActive())
        _timer.stop();

    if( _exit_requested() )
        emit finishedSignal();
}

void BallMover::notBusyQuit()
{
    if( _timer.isActive() )
        _timer.stop();

    emit finishedSignal();
}
