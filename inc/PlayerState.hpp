#ifndef _PlayerState
#define _PlayerState

#include <QObject>

class PlayerState : public QObject
{
Q_OBJECT

public:
  PlayerState(const int & racketPosition=0,
	      const int & concededPoints=0,
	      const bool & failState=false);

private:
  int _racketPosition;
  int _concededPoints;
  bool _failState;
}

#endif
