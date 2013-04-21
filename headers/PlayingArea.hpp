#ifndef _PlayingArea
#define _PlayingArea

#include<cmath>

#include <QVector>
#include <QList>
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QRectF>

#include "Trigo.hpp"
#include "Random.hpp"
#include "Lockable.hpp"

class PlayingArea : public Lockable
{
public:
    typedef QGraphicsLineItem Linear;
    typedef QGraphicsEllipseItem Ball;
    typedef QGraphicsRectItem Scene;

    PlayingArea(const qint32 & nbPlayers=6,
                const qint32 & renderAreaWidth=600,
                const qreal & ballRadius=5);

    void build();

    QPointF centerPoint()const;
    qreal centerAngle()const;
    qreal areaWidth()const;

    Scene * scene();

    bool isCage(Linear *item)const;
    bool isRacket(Linear *item)const;
    bool isWall(Linear *item)const;

    qint32 nbPlayers()const;
    qint32 nbCages()const;
    qint32 nbRackets()const;
    qint32 nbWalls()const;
    QPointF ballPos()const;
    QRectF ballRect()const;

    qint32 cageIndex(Linear *item)const;
    qint32 racketIndex(Linear * item)const;
    qint32 wallIndex(Linear * item)const;

    Linear * wall(const qint32 & index) const;
    Linear * cage(const qint32 & index) const;
    Linear * racket(const qint32 & index) const;
    QLineF racketLine(const qint32 & index)const;

    qreal getWallRotation(const qint32 & wallIndex);

    bool collisionHappened();
    Linear * getBallCollider()const;

    void rebuild(const qint32 & nbPlayers=6,
                 const qreal & renderAreaWidth=600.,
                 const qreal & ballRadius=5);

    void setBallRadius(const qreal & ballRadius);
    void resetBallPos();
    void rotateBallDirection(const qreal & alpha);
    void mirrorBallDirection(Linear * axis);
    void moveBall(const qreal & dx, const qreal &dy);
    void moveBall();

    void moveRacket(const qint32 &racketIndex,
                    const qreal & delta);

    void setRacketLine(const qint32 &racketIndex,
                       const QLineF & line);
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

    Scene _scene;
    qreal _ballRadius;
    Ball _ball;
    QLineF _ballDirection;
    QVector<Linear*> _cages;
    QVector<Linear*> _rackets;
    QVector<Linear*> _walls;

    Linear * _ballCollider;

    static const qreal _wallRatio;
    static const qreal _cageRatio;
    static const qreal _racketRatio;
    static const qreal _racketToWallSpace;
    static const qreal _ballTranslateQuantum;

    void _clear();
    void _generate_area(const qint32 & nbPlayers);
    void _set_line_position(QLineF & line,
                            const qreal & dx,
                            const qreal & dy,
                            const qreal & alpha);
    void _set_ball_random_direction();
    bool _collisionIn(QVector<Linear*> & items);
};

#endif
