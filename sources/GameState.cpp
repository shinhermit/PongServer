#include "GameState.hpp"

GameState::GameState(const PongTypes::E_GameState & initialState):
    _state(initialState),
    _loserIndex(-1),
    _downCounter(0),
    _nbPlayers(0),
    _nbActive(0)
{}

GameState::GameState(const GameState &source):
    _state(source._state),
    _loserIndex(source._loserIndex),
    _downCounter(source._downCounter),
    _nbPlayers(source._nbPlayers),
    _nbActive(source._nbActive)
{}

GameState &GameState::operator =(const GameState &source)
{
    _state = source._state;
    _loserIndex = source._loserIndex;
    _downCounter = source._downCounter;
    _nbPlayers = source._nbPlayers;
    _nbActive = source._nbActive;

    return *this;
}


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

void GameState::setLoserIndex(const qint32 &index)
{
    if(index < -1)
        qDebug() << "GameState::setLoserIndex: loserIndex must be positive or -1 (no loser)";

    _loserIndex = index;
}

void GameState::setDownCounter(const qint32 &value)
{
    _downCounter = ::abs(value);
}

void GameState::setNbPlayers(const qint32 &nbPlayers)
{
    if(nbPlayers < 0)
        qDebug() << "GameState::setNbPlayers: invalid negative value";

    _nbPlayers = nbPlayers;
    if(_nbActive > _nbPlayers)
        _nbActive = _nbPlayers;
}

void GameState::setNbActive(const qint32 &nbActive)
{
    if(nbActive < 0)
        qDebug() << "GameState::setNBActive: invalid negative value";

    if(nbActive > _nbPlayers)
        qDebug() << "GameState::setNBActive: warning: more active players than connected players";

    _nbActive = nbActive;
}

void GameState::incNbPlayers()
{
    ++_nbPlayers;
}

void GameState::decNbPlayers()
{
    --_nbPlayers;
}

void GameState::incNbActive()
{
    ++_nbActive;
}

void GameState::decNbActive()
{
    -- _nbActive;
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

const qint32 &GameState::nbActive() const
{
    return _nbActive;
}

