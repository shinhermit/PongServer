#ifndef _GameState
#define _GameState

#include <cmath>

#include <QDebug>

#include "PongTypes.hpp"
#include "Lockable.hpp"

class GameState : public Lockable
{
  public:
  GameState(const PongTypes::E_GameState & initialState=PongTypes::NOPARTY);
  GameState(const GameState & source);
  GameState & operator=(const GameState & source);

  void setGameOver(const qint32 & cageIndex);
  void setPaused();
  void setExitRequested();
  void setWaitingServer();
  void setInitializing();
  void setRunning();
  void setNoParty();
  void setStateError();
  void setStartRequested();
  void setResetRequested();

  void setLoserIndex(const qint32 & index);
  void setDownCounter(const qint32 & value);
  void setNbPlayers(const qint32 & nbPlayers);
  void setNbActive(const qint32 & nbActive);
  void incNbPlayers();
  void decNbPlayers();
  void incNbActive();
  void decNbActive();

  const PongTypes::E_GameState & state()const;
  const qint32 & loserIndex()const;
  const qint32 & downCounter()const;
  const qint32  & nbPlayers()const;
  const qint32 & nbActive()const;

private:
  PongTypes::E_GameState _state;
  qint32 _loserIndex;
  qint32 _downCounter;
  qint32 _nbPlayers;
  qint32 _nbActive;
};

#endif
