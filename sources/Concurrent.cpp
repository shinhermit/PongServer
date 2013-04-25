#include "Concurrent.hpp"

Concurrent::Concurrent(QObject * parent):
    QObject(parent),
    _I_locked_gameState(false),
    _I_locked_playingArea(false),
    _I_locked_playersStates(false)
{}

void Concurrent::lockGameState()
{
    if(!_I_locked_gameState)
    {
        PongShared::gameState.lock();
        _I_locked_gameState = true;
    }
}

void Concurrent::lockPlayingArea()
{
    if(!_I_locked_playingArea)
    {
        PongShared::playingArea.lock();
        _I_locked_playingArea= true;
    }
}

void Concurrent::lockPlayersStates()
{
    if(!_I_locked_playersStates)
    {
        PongShared::playersStatesMutex.lock();
        _I_locked_playersStates= true;
    }
}

void Concurrent::unlockGameState()
{
    if(_I_locked_gameState)
    {
        PongShared::gameState.unlock();
        _I_locked_gameState = false;
    }
}

void Concurrent::unlockPlayingArea()
{
    if(_I_locked_playingArea)
    {
        PongShared::playingArea.unlock();
        _I_locked_playingArea = false;
    }
}

void Concurrent::unlockPlayersStates()
{
    if(_I_locked_playersStates)
    {
        PongShared::playersStatesMutex.unlock();
        _I_locked_playersStates = false;
    }
}

void Concurrent::notBusyQuit()
{
    emit finishedSignal();
}

bool Concurrent::_exit_requested()
{
    bool requested;

    lockGameState();
    requested  = ( PongShared::gameState.state() == PongTypes::EXIT_REQUESTED );
    unlockGameState();

    return requested;
}
