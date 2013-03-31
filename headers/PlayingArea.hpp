#ifndef _PlayingArea
#define _PlayingArea

#include<cmath>

#include <QVector>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QDebug>

#include "Trigo.hpp"
#include "Random.hpp"
#include "Lockable.hpp"

class PlayingArea : public Lockable
{
private:
    void _clear_scene();
    void _generate_area(const int & nbPlayers);
    void _generate_area();
    void _set_ball_random_direction();
    void _init_ball(const QRectF &ballRect);

public:
    PlayingArea(const int & nbPlayers,
                const qreal & renderAreaWidth,
                const QRectF &ballRect);

    PlayingArea(const int & nbPlayers=3,
                const qreal &renderAreaWidth=600.);

    const qreal & centerAngle()const;
    qreal areaWidth()const;
    QGraphicsScene * scene();

    bool isCage(QGraphicsItem *item)const;
    bool isRacket(QGraphicsItem *item)const;
    bool isWall(QGraphicsItem *item)const;

    int cageIndex(QGraphicsItem *item)const;
    int racketIndex(QGraphicsItem * item)const;
    int wallIndex(QGraphicsItem *item)const;

    QGraphicsLineItem * wall(const int & index) const;
    QGraphicsLineItem * cage(const int & index) const;
    QGraphicsLineItem * racket(const int & index) const;

    qreal getWallRotation(const int & wallIndex);

    void reset(const int & nbPlayers,
               const qreal & renderAreaWidth,
               const QRectF &ballRect);
    void reset(const int & nbPlayers=3,
               const qreal & renderAreaWidth=600.);

    void resetBallPos();
    void rotateBallDirection(const qreal & alpha);
    void mirrorBallDirection(QGraphicsLineItem *axis);
    void moveBall(const qreal & dx, const qreal &dy);
    void moveBall();
    QList<QGraphicsItem*> getBallColliders()const;

    void moveRacket(const int &racketIndex,
                    const qreal & delta);

    void setRacketAbss(const int & racketIndex,
                       const qreal & abss);
    void setRacketOrd(const int & racketIndex,
                      const qreal & ord);
    void setRacketCoord(const int & racketIndex,
                        const qreal & x,
                        const qreal & y);

    void removeCage(const int & cageIndex);
    void removeRacket(const int & racketIndex);
    void removeWall(const int & wallIndex);

private:
    int _nbPlayers;
    qreal _renderAreaWidth;
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
};

#endif
