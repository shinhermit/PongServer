#ifndef _GameState
#define _GameState

#include <cmath>
#include <stdexcept>

#include "PongTypes.hpp"
#include "Lockable.hpp"

class GameState : public Lockable
{
  public:
  GameState();

  void setGameOver(const qint32 & cageIndex);
  void setPaused();
  void setExited();
  void setWaitingServer();
  void setInitializing();
  void setRunning();
  void setNoParty();

  void setLoserIndex(const qint32 & index) throw(std::invalid_argument);
  void setDownCounter(const qint32 & value);
  void setNbPlayers(const qint32 & nbPlayers) throw(std::invalid_argument);

  const PongTypes::E_GameState & state()const;
  const qint32 & loserIndex()const;
  const qint32 & downCounter()const;
  const qint32  & nbPlayers()const;

private:
  PongTypes::E_GameState _state;
  qint32 _loserIndex;
  qint32 _downCounter;
  qint32 _nbPlayers;
};

#endif
