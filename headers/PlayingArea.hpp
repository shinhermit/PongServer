#ifndef _PlayingArea
#define _PlayingArea

#include<cmath>

#include <QVector>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QDebug>
#include <QPointF>

#include "Trigo.hpp"
#include "Random.hpp"
#include "Lockable.hpp"

class PlayingArea : public Lockable
{
public:
    PlayingArea(const qint32 & nbPlayers,
                const qint32 & renderAreaWidth,
                const QRectF &ballRect);

    PlayingArea(const qint32 & nbPlayers=2,
                const qint32 &renderAreaWidth=600);

    const qreal & centerAngle()const;
    qreal areaWidth()const;
    QGraphicsScene * scene();

    bool isCage(QGraphicsItem *item)const;
    bool isRacket(QGraphicsItem *item)const;
    bool isWall(QGraphicsItem *item)const;

    qint32 nbCages()const;
    qint32 nbRackets()const;
    qint32 nbWalls()const;
    QPointF ballPos()const;

    qint32 cageIndex(QGraphicsItem *item)const;
    qint32 racketIndex(QGraphicsItem * item)const;
    qint32 wallIndex(QGraphicsItem *item)const;

    QGraphicsLineItem * wall(const qint32 & index) const;
    QGraphicsLineItem * cage(const qint32 & index) const;
    QGraphicsLineItem * racket(const qint32 & index) const;

    qreal getWallRotation(const qint32 & wallIndex);

    void rebuild(const qint32 & nbPlayers,
               const qreal & renderAreaWidth,
               const QRectF &ballRect);
    void rebuild(const qint32 & nbPlayers=0,
               const qreal & renderAreaWidth=600.);

    void resetBallPos();
    void rotateBallDirection(const qreal & alpha);
    void mirrorBallDirection(QGraphicsLineItem *axis);
    void moveBall(const qreal & dx, const qreal &dy);
    void moveBall();
    QList<QGraphicsItem*> getBallColliders()const;

    void moveRacket(const qint32 &racketIndex,
                    const qreal & delta);

    void setRacketAbss(const qint32 & racketIndex,
                       const qreal & abss);
    void setRacketOrd(const qint32 & racketIndex,
                      const qreal & ord);
    void setRacketCoord(const qint32 & racketIndex,
                        const qreal & x,
                        const qreal & y);

    void removeCage(const qint32 & cageIndex);
    void removeRacket(const qint32 & racketIndex);
    void removeWall(const qint32 & wallIndex);

private:
    qint32 _nbPlayers;
    qint32 _renderAreaWidth;
    qreal _centerAngle;
    MathJ::Random _rndGen;

    QGraphicsScene * _scene;
    QGraphicsEllipseItem * _ball;
    QLineF _ballDirection;
    QVector<QGraphicsLineItem*> _cages;
    QVector<QGraphicsLineItem*> _rackets;
    QVector<QGraphicsLineItem*> _walls;

    static const qreal _wallRatio;
    static const qreal _cageRatio;
    static const qreal _racketRatio;
    static const qreal _racketToWallSpace;
    static const qreal _penWidth;
    static const qreal _ballRadius;
    static const qreal _ballTranslateQuantum;

    void _clear_scene();
    void _generate_area(const qint32 & nbPlayers);
    void _generate_area();
    void _set_ball_random_direction();
    void _init_ball(const QRectF &ballRect);
};

#endif
