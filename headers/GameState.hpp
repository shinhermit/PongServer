#ifndef _GameState
#define _GameState

#include <cmath>
#include <stdexcept>
#include <QObject>

#include "PongTypes.hpp"
#include "Lockable.hpp"

class GameState : public QObject
{
public:
  GameState(const PongTypes::E_GameState & initialState=PongTypes::NOPARTY);
  GameState(const GameState & source);
  GameState & operator=(const GameState & source);

  const PongTypes::E_GameState & state()const;
  const qint32 & loserIndex()const;
  const qint32 & downCounter()const;
  const qint32  & nbPlayers()const;

  void setGameOver(const qint32 & cageIndex);
  void setPaused();
  void setExitRequested();
  void setWaitingServer();
  void setInitializing();
  void setRunning();
  void setNoParty();
  void setStateError();
  void setStartRequested();

  void setLoserIndex(const qint32 & index) throw(std::invalid_argument);
  void setDownCounter(const qint32 & value);
  void setNbPlayers(const qint32 & nbPlayers) throw(std::invalid_argument);


//synchronization

  void askShareState();
  bool upToDate()const;

public slots:
  void setState(
          PongTypes::E_GameState state,
          qint32 loserIndex,
          qint32 downCounter,
          qint32 nbPlayers
          );

  void shareState();

signals:
  void setStateDemand(
          PongTypes::E_GameState state,
          qint32 loserIndex,
          qint32 downCounter,
          qint32 nbPlayers
          );

  void shareStateDemand();

private:
  PongTypes::E_GameState _state;
  qint32 _loserIndex;
  qint32 _downCounter;
  qint32 _nbPlayers;
  bool _upToDate;
};

#endif
