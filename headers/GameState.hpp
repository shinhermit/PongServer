#ifndef _GameState
#define _GameState

#include "PongTypes.hpp"
#include "Lockable.hpp"

class GameState : public Lockable
{
  public:
  GameState();

  void setGameOver(const int & cageIndex);
  void setPaused();
  void setExited();
  void setInitializing();
  void setRunning();
  void setNoParty();

  const PongTypes::E_GameState & state()const;
  const int & loserIndex()const;

private:
  PongTypes::E_GameState _state;
  int _loserIndex;
};

#endif
