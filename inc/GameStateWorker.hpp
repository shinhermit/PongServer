#ifndef _GameStateWorker
#define _GameStateWorker

#include <QObject>
#include <QVector>

#include "PlayingArea.hpp"
#include "PlayerState.hpp"

class GameStateWorker : public QObject
{
  Q_OBJECT

  public:
  GameStateWorker(PlayingArea & playingArea,
		  Qvector<PlayerState> & playersStates,
		  bool stopped);

public slots:
  void checkState();
}

#endif
