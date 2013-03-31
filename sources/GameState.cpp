#include "GameState.hpp"

GameState::GameState():
    _state(PongTypes::NOPARTY),
    _loserIndex(-1)
{}


void GameState::setGameOver(const int &cageIndex)
{
    _state = PongTypes::GAMEOVER;
    _loserIndex = cageIndex;
}

void GameState::setPaused()
{
    _state = PongTypes::PAUSED;
}

void GameState::setExited()
{
    _state = PongTypes::EXITED;
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

const PongTypes::E_GameState &GameState::state() const
{
    return _state;
}

const int & GameState::loserIndex() const
{
    return _loserIndex;
}

