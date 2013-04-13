#include "GameState.hpp"

GameState::GameState(const PongTypes::E_GameState & initialState):
    _state(initialState),
    _loserIndex(-1),
    _downCounter(0),
    _nbPlayers(0)
{}


void GameState::setGameOver(const qint32 &cageIndex)
{
    _state = PongTypes::GAMEOVER;
    _loserIndex = cageIndex;
}

void GameState::setPaused()
{
    _state = PongTypes::PAUSED;
}

void GameState::setExitRequested()
{
    _state = PongTypes::EXIT_REQUESTED;
}

void GameState::setWaitingServer()
{
    _state = PongTypes::WAITING_SERVER;
}

void GameState::setInitializing()
{
    _state = PongTypes::INITIALIZING;
}

void GameState::setRunning()
{
    _state = PongTypes::RUNNING;
}

void GameState::setNoParty()
{
    _state = PongTypes::NOPARTY;
}

void GameState::setStateError()
{
    _state = PongTypes::STATE_ERROR;
}

void GameState::setStartRequested()
{
    _state = PongTypes::START_REQUESTED;
}

void GameState::setLoserIndex(const qint32 &index) throw(std::invalid_argument)
{
    if(index < -1)
        throw std::invalid_argument("GameState::setLoserIndex: index must be positive or -1 (no loser)");

    _loserIndex = index;
}

void GameState::setDownCounter(const qint32 &value)
{
    _downCounter = ::abs(value);
}

void GameState::setNbPlayers(const qint32 &nbPlayers) throw(std::invalid_argument)
{
    if(nbPlayers < 0)
        throw std::invalid_argument("GameState::setNbPlayers: invalid negative value");

    _nbPlayers = nbPlayers;
}

const PongTypes::E_GameState &GameState::state() const
{
    return _state;
}

const qint32 &GameState::loserIndex() const
{
    return _loserIndex;
}

const qint32 &GameState::downCounter() const
{
    return _downCounter;
}

const qint32 &GameState::nbPlayers() const
{
    return _nbPlayers;
}

