#ifndef _PlayingArea
#define _PlayingArea

#include<cmath>

#include <QVector>
#include <QPointF>
//#include <QLineF>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>

#include "MathUtils.hpp"

class PlayingArea
{
private:
    void _generate_area();

public:
    PlayingArea(const int & nbPlayers,
                const qreal & renderAreaWidth,
                const QRectF &ball);

    PlayingArea(const int & nbPlayers=3,
                const qreal &renderAreaWidth=600.);

    const qreal & centerAngle()const;
    qreal areaWidth()const;
    QGraphicsScene * scene();

    bool isCage(const QGraphicsItem & item)const;
    bool isRacket(const QGraphicsItem & item)const;
    bool isWall(const QGraphicsItem & item)const;

    int cageIndex(const QGraphicsItem & item)const;
    int racketIndex(const QGraphicsItem & item)const;
    int wallIndex(const QGraphicsItem & item)const;

    qreal getWallRotation(const int & wallIndex);

    void update();
    void reset(const int & nbPlayers,
               const qreal & renderAreaWidth,
               const QRectF &ball);
    void reset(const int & nbPlayers=3,
               const qreal & renderAreaWidth=600.);

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
    int _nbPlayers;
    qreal _renderAreaWidth;
    qreal _centerAngle;

    QGraphicsScene * _scene;
    QGraphicsRectItem * _ball;
    QVector<QGraphicsLineItem*> _cages;
    QVector<QGraphicsLineItem*> _rackets;
    QVector<QGraphicsLineItem*> _walls;

    static const qreal _wallRatio;
    static const qreal _cageRatio;
    static const qreal _racketRatio;
    static const qreal _racketToWallSpace;
    static const qreal _penWidth;
};

#endif
