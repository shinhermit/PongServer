#ifndef _PlayingArea
#define _PlayingArea

#include <QVector>
#include <QPointF>
#include <QGraphicsItem>

#include "Ball.hpp"
#include "Racket.hpp"
#include "Cage.hpp"
#include "Wall.hpp"

class PlayingArea
{
public:
  PlayingArea(const int & renderAreaWidth,
	      const Ball & ball);

  const QPointF & centerPoint()const;
  const int & areaWidth()const;

  bool isCage(const QGraphicsItem & item)const;
  bool isRacket(const QGraphicsItem & item)const;
  bool isWall(const QGraphicsItem & item)const;

  int cageIndex(const QGraphicsItem & item)const;
  int racketIndex(const QGraphicsItem & item)const;
  int wallIndex(const QGraphicsItem & item)const;

  qreal getWallRotation(const int & wallIndex);

  void update();

  void resetBall();
  void rotateBallDirection(const qreal & alpha);
  void mirrorBallDirection(const qreal & m11,
			   const qreal & m12,
			   const qreal & m21,
			   const qreal & m22);
  void moveBall(const qreal & delta=20);
  QList<QGraphicsItem*> getBallColliders()const;

  void setRacketAbss(const int & racketIndex,
		    const qreal & abss);
  void setRacketOrd(const int & racketIndex,
		    const qreal & ord);
  void setRacketCoord(const int & racketIndex,
		      const QPointF & p1,
		      const QPointF & p2);

  void removeCage(const int & cageIndex);
  void removeRacket(const int & racketIndex);
  void removeWall(const int & wallIndex);

private:
  int _renderAreaWidth;
  Ball _ball;
  QVector<Cage> _cages;
  QVector<Racket> _rackets;
  QVector<Wall> _walls;
};

#endif
