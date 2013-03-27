#ifndef _PlayerState
#define _PlayerState

#include <QObject>
#include <QLine>

#include "PongTypes.hpp"

class PlayerState
{
public:
  PlayerState(const int & racketPosition=0,
	      const int & concededPoints=0,
	      const bool & failState=false
	      const int & initialCredit=3);

  void decreaseCredit(const int & step=1);
  void setState(const PongTypes::E_PlayerState & state);

  const QLine & relativeRacket()const;
  const int & credit()const;

private:
  QLine _relativeRacket;
  int _credit;
  bool _failState;
}

#endif
