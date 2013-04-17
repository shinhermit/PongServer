#include "PlayingArea.hpp"

using namespace MathJ;

const qreal PlayingArea::_wallRatio = 1./4;
const qreal PlayingArea::_cageRatio = 1./2;
const qreal PlayingArea::_racketRatio = 1./6;
const qreal PlayingArea::_racketToWallSpace = 20;
const qreal PlayingArea::_penWidth = 5;
const qreal PlayingArea::_ballRadius = 5;
const qreal PlayingArea::_ballTranslateQuantum = 1;

PlayingArea::PlayingArea(const qint32 & nbPlayers,
                         const qint32 & renderAreaWidth,
                         const QRectF & ballRect,
                         QGraphicsScene * scene):
    _nbPlayers(nbPlayers),
    _renderAreaWidth(renderAreaWidth),
    _centerAngle(360/nbPlayers),
    _scene(scene),
    _ballDirection(0,0, _ballTranslateQuantum, 0)
{
    _init_ball(ballRect);

    _generate_area();
}

PlayingArea::PlayingArea(const qint32 & nbPlayers,
                         const qint32 & renderAreaWidth,
                         QGraphicsScene * scene):
    _nbPlayers(nbPlayers),
    _renderAreaWidth(renderAreaWidth),
    _centerAngle(360/nbPlayers),
    _scene(scene),
    _ballDirection(0,0, _ballTranslateQuantum, 0)
{}

void PlayingArea::setScene(QGraphicsScene *scene)
{
    _scene = scene;
}

void PlayingArea::build()
{
    _init_ball( QRectF( -_ballRadius, -_ballRadius, 2*_ballRadius, 2*_ballRadius ) );
    _generate_area();
}

const qreal &PlayingArea::centerAngle() const
{
    return _centerAngle;
}

qreal PlayingArea::areaWidth()const
{
    return _renderAreaWidth;
}

QGraphicsScene * PlayingArea::scene()
{
    return _scene;
}

bool PlayingArea::isCage(QGraphicsItem *item)const
{
    return _cages.indexOf(qgraphicsitem_cast<QGraphicsLineItem*>(item)) != -1;
}

bool PlayingArea::isRacket(QGraphicsItem *item)const
{
    return _rackets.indexOf(qgraphicsitem_cast<QGraphicsLineItem*>(item)) != -1;
}

bool PlayingArea::isWall(QGraphicsItem *item)const
{
    return _walls.indexOf(qgraphicsitem_cast<QGraphicsLineItem*>(item)) != -1;
}

qint32 PlayingArea::nbCages()const
{
    return _cages.size();
}

qint32 PlayingArea::nbRackets()const
{
    return _rackets.size();
}

qint32 PlayingArea::nbWalls()const
{
    return _walls.size();
}

QPointF PlayingArea::ballPos() const
{
    QPointF relPos, absPos;

    relPos = _ball->pos();
    absPos = _ball->mapToScene( relPos );

    return absPos;
}

qint32 PlayingArea::cageIndex(QGraphicsItem *item)const
{
    return _cages.indexOf(qgraphicsitem_cast<QGraphicsLineItem*>(item));
}

qint32 PlayingArea::racketIndex(QGraphicsItem *item)const
{
    return _rackets.indexOf(qgraphicsitem_cast<QGraphicsLineItem*>(item));
}

qint32 PlayingArea::wallIndex(QGraphicsItem *item)const
{
    return _walls.indexOf(qgraphicsitem_cast<QGraphicsLineItem*>(item));
}

QGraphicsLineItem *PlayingArea::wall(const qint32 &index)const
{
    return _walls[index];
}

QGraphicsLineItem *PlayingArea::cage(const qint32 &index) const
{
    return _cages[index];
}

QGraphicsLineItem *PlayingArea::racket(const qint32 &index) const
{
    return _rackets[index];
}

qreal PlayingArea::getWallRotation(const qint32 & wallIndex)
{
    return _walls[wallIndex]->rotation();
}

void PlayingArea::rebuild(const qint32 &nbPlayers,
                        const qreal &renderAreaWidth,
                        const QRectF &ballRect)
{
    _nbPlayers = nbPlayers;
    _renderAreaWidth = renderAreaWidth;
    _ball->setRect(ballRect);
    resetBallPos();
    _clear_scene();
    _generate_area();
}

void PlayingArea::rebuild(const qint32 &nbPlayers,
                        const qreal &renderAreaWidth)
{
    rebuild(nbPlayers, renderAreaWidth, QRectF( -_ballRadius, -_ballRadius, 2*_ballRadius, 2*_ballRadius ) );
}

void PlayingArea::resetBallPos()
{
    QPointF pt = _ball->mapFromScene(0,0);
    _ball->setPos(pt);
    _set_ball_random_direction();
}

void PlayingArea::rotateBallDirection(const qreal & alpha)
{
    _ballDirection.setAngle(_ballDirection.angle()+alpha);
}

void PlayingArea::mirrorBallDirection(QGraphicsLineItem * axis)
{
    QPointF endPoint = _ball->mapToItem( axis, _ballDirection.p2() );

    endPoint.setY( 0 - endPoint.y() );

    endPoint = _ball->mapFromItem( axis, endPoint);
    QLineF patron(_ballDirection.p1(), endPoint);
    _ballDirection.setAngle(patron.angle());
}

void PlayingArea::moveBall(const qreal & dx, const qreal & dy)
{
    _ball->moveBy(dx, dy);
    _ballDirection.translate( QPointF(dx,dy) );
}

void PlayingArea::moveBall()
{
    moveBall( _ballDirection.dx(), _ballDirection.dy() );
}

QList<QGraphicsItem*> PlayingArea::getBallColliders()const
{
    return _scene->collidingItems(_ball);
}

void PlayingArea::moveRacket(const qint32 & racketIndex,
                             const qreal & delta)
{
    if( 0 <= racketIndex && racketIndex < _rackets.size() )
    {
        qreal deviationAngle = _rackets[racketIndex]->rotation();
        qreal dx = delta * ::cos( MathJ::Trigo::DegreeToRadian(deviationAngle) );
        qreal dy = delta * ::sin(MathJ::Trigo::DegreeToRadian(deviationAngle) );


        _rackets[racketIndex]->moveBy(dx, dy);
    }

    else
        qDebug() << "PlayingArea::moveRacket : erreur index racket " << racketIndex << endl;
}

void PlayingArea::setRacketAbss(const qint32 & racketIndex,
                                const qreal & abss)
{
    if( 0 <= racketIndex && racketIndex < _rackets.size() )
    {
        _rackets[racketIndex]->setX(abss);
    }
}

void PlayingArea::setRacketOrd(const qint32 & racketIndex,
                               const qreal & ord)
{
    if( 0 <= racketIndex && racketIndex < _rackets.size() )
    {
        _rackets[racketIndex]->setY(ord);
    }
}

void PlayingArea::setRacketCoord(const qint32 & racketIndex,
                                 const qreal & x,
                                 const qreal & y)
{
    setRacketAbss(racketIndex, x);
    setRacketOrd(racketIndex, y);
}

void PlayingArea::removeCage(const qint32 & cageIndex)
{
    int index;

    if( 0 <= cageIndex && cageIndex < _cages.size() )
    {

        QPointF p1 = _cages[cageIndex]->line().p1();
        QPointF p2 = _cages[cageIndex]->line().p2();

        p1 = _cages[cageIndex]->mapToScene(p1);
        p2 = _cages[cageIndex]->mapToScene(p2);

        index = _walls.size();
        _walls.push_back( new QGraphicsLineItem(QLineF(p1, p2)) );
        _scene->addItem(_walls[index]);
        _walls[index]->setPen(QPen(Qt::blue, _penWidth, Qt::SolidLine));

        _scene->removeItem(_cages[cageIndex]);
        _cages.erase(_cages.begin()+cageIndex);
    }
}

void PlayingArea::removeRacket(const qint32 & racketIndex)
{
    if( 0 <= racketIndex && racketIndex < _rackets.size() )
    {
        _scene->removeItem(_rackets[racketIndex]);
        _rackets.erase(_rackets.begin()+racketIndex);
    }
}

void PlayingArea::removeWall(const qint32 & wallIndex)
{
    if( 0 <= wallIndex && wallIndex < _walls.size() && _nbPlayers >= 3)
    {
        _nbPlayers -= 1;
        resetBallPos();
        _clear_scene();
        _generate_area();
    }

}

void PlayingArea::_clear_scene()
{
    for(qint32 i=0; i<_walls.size(); ++i)
    {
        _scene->removeItem(_walls[i]);
    }

    for(qint32 i=0; i<_rackets.size(); ++i)
    {
        _scene->removeItem(_rackets[i]);
    }

    for(qint32 i=0; i<_cages.size(); ++i)
    {
        _scene->removeItem(_cages[i]);
    }

    _walls.clear();
    _rackets.clear();
    _cages.clear();
}

void PlayingArea::_generate_area(const qint32 & nbPlayers)
{
    qint32 j;
    qreal dx, dy;
    qreal radius = _renderAreaWidth / 2;
    qreal alpha = 360 / nbPlayers;
    qreal beta = (180 - alpha) / 2;

    qreal sideLength = radius * ( ::sin(Trigo::DegreeToRadian(alpha)) / ::sin(Trigo::DegreeToRadian(beta)) );
    qreal wallLength = sideLength * _wallRatio;
    qreal racketLength = sideLength * _racketRatio;
    qreal cageLength = sideLength * _cageRatio;
    qreal altitude = radius * ::cos( Trigo::DegreeToRadian(alpha/2) );

    j = 0;
    for(qint32 i=0; i < nbPlayers; ++i)
    {
        _walls.push_back( new QGraphicsLineItem(0.,altitude, -wallLength,altitude) );
        _walls.push_back( new QGraphicsLineItem(0.,altitude, -wallLength,altitude) );
        _rackets.push_back( new QGraphicsLineItem(racketLength/2,altitude, -racketLength/2,altitude) );
        _cages.push_back( new QGraphicsLineItem(0.,altitude, -cageLength,altitude) );

        dx = sideLength / 2;
        dy = 0;
        _scene->addItem(_walls[j]);
        _walls[j]->moveBy(dx, dy);
        _walls[j]->setPen(QPen(Qt::blue, _penWidth, Qt::SolidLine));
        _walls[j]->setTransformOriginPoint(_walls[j]->mapFromScene(0,0));
        _walls[j]->setRotation(_walls[j]->rotation() + i*alpha);
        ++j;

        dx -= wallLength;
        _scene->addItem(_cages[i]);
        _cages[i]->moveBy(dx, dy);
        _cages[i]->setTransformOriginPoint(_cages[i]->mapFromScene(0,0));
        _cages[i]->setRotation(_cages[i]->rotation() + i*alpha);

        dy = -_racketToWallSpace;
        _scene->addItem(_rackets[i]);
        _rackets[i]->moveBy(0, dy);
        _rackets[i]->setPen( QPen(Qt::darkRed, _penWidth, Qt::SolidLine) );
        _rackets[i]->setTransformOriginPoint(_rackets[i]->mapFromScene(0,0));
        _rackets[i]->setRotation(_rackets[i]->rotation() + i*alpha);

        dy = 0;
        dx -= cageLength;
        _scene->addItem(_walls[j]);
        _walls[j]->moveBy(dx, dy);
        _walls[j]->setPen(QPen(Qt::blue, _penWidth, Qt::SolidLine));
        _walls[j]->setTransformOriginPoint(_walls[j]->mapFromScene(0,0));
        _walls[j]->setRotation(_walls[j]->rotation() + i*alpha);

        ++j;
    }
}

void PlayingArea::_generate_area()
{
    if(_nbPlayers == 2)
    {
        _generate_area(4);

        removeRacket(3);
        removeCage(3);

        removeCage(1);
        removeRacket(1);
    }

    else
    {
        _generate_area(_nbPlayers);
    }
}

void PlayingArea::_set_ball_random_direction()
{
    qreal alpha = _rndGen.randomIntbeetween(0, 360);

    _ballDirection.setAngle(_ballDirection.angle()+alpha);
}

void PlayingArea::_init_ball(const QRectF & ballRect)
{
    _ball = new QGraphicsEllipseItem(ballRect);
    _scene->addItem(_ball);
    _ball->setBrush( Qt::red );
    _set_ball_random_direction();
}
