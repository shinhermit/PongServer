#ifndef _GameState
#define _GameState

#include "PongTypes.hpp"

class GameState
{
  public:
  GameState();

  void setGameOver(const int & cageIndex);
  void setPaused();
  void setExited();
  void setRunning();
  void setNoParty();

  const PongTypes::E_GameState & state()const;
  const int & loserIndex()const;

private:
  PongTypes::E_GameState _state;
  int _loserIndex;
};

#endif
