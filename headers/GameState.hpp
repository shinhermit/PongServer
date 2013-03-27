#ifndef _GameState
#define _GameState

#include "PongTypes.hpp"

class GameState
{
  public:
  GameState();

  void setGameOverState(const int & cageIndex);

  const PongTypes::E_GameState & state()const;

private:
  PongTypes::E_GameState _state;

  int _gameOverLoserIndex;
};

#endif
