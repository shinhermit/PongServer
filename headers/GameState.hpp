#ifndef _GameState
#define _GameState

#include "PongTypes.hpp"

class GameState
{
  public:
  GameState();

  void setGameOverState(cageIndex);

  const PongTypes::E_GameState & state()const;

private:
  PongTypes::GameState _state;

  int _gameOverLoserIndex;
};

#endif
