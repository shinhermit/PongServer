#include "PlayingArea.hpp"

using namespace MathJ;

const qreal PlayingArea::_wallRatio = 1./4;
const qreal PlayingArea::_cageRatio = 1./2;
const qreal PlayingArea::_racketRatio = 1./6;
const qreal PlayingArea::_racketToWallSpace = 20;
const qreal PlayingArea::_ballTranslateQuantum = 1;

PlayingArea::PlayingArea(const qint32 & nbPlayers,
                         const qint32 & renderAreaWidth,
                         const qreal & ballRadius):
    _nbPlayers(nbPlayers),
    _renderAreaWidth(renderAreaWidth),
    _centerAngle(360/nbPlayers),
    _scene(-renderAreaWidth/2, -renderAreaWidth/2, renderAreaWidth, renderAreaWidth),
    _ballRadius(ballRadius),
    _ball(-ballRadius, -ballRadius, 2*ballRadius, 2*ballRadius),
    _ballDirection(0,0, _ballTranslateQuantum, 0),
    _ballCollider(0)
{
    _ball.setParentItem(&_scene);
}


void PlayingArea::build()
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

QPointF PlayingArea::centerPoint()const
{
    return _scene.pos();
}

qreal PlayingArea::centerAngle()const
{
    return _centerAngle;
}

qreal PlayingArea::areaWidth()const
{
    return _renderAreaWidth;
}

PlayingArea::Scene * PlayingArea::scene()
{
    return &_scene;
}

bool PlayingArea::isCage(PlayingArea::Linear * item)const
{
    return _cages.indexOf(item) != -1;
}

bool PlayingArea::isRacket(PlayingArea::Linear *item)const
{
    return _rackets.indexOf(item) != -1;
}

bool PlayingArea::isWall(PlayingArea::Linear*item)const
{
    return _walls.indexOf(item) != -1;
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
    return _ball.pos();
}

QRectF PlayingArea::ballRect() const
{
    return _ball.rect();
}

qint32 PlayingArea::cageIndex(PlayingArea::Linear * item)const
{
    return _cages.indexOf(item);
}

qint32 PlayingArea::racketIndex(PlayingArea::Linear * item)const
{
    return _rackets.indexOf(item);
}

qint32 PlayingArea::wallIndex(PlayingArea::Linear * item)const
{
    return _walls.indexOf(item);
}

PlayingArea::Linear * PlayingArea::wall(const qint32 &index)const
{
    if( 0 <= index && index < _walls.size() )
        return _walls[index];

    else
    {
        qDebug() << "PlayingArea::wall(i) : given index " << index << " is out of range" << endl;
        return 0;
    }
}

PlayingArea::Linear * PlayingArea::cage(const qint32 &index) const
{
    if( 0 <= index && index < _cages.size() )
        return _cages[index];

    else
    {
        qDebug() << "PlayingArea::cage(i) : given index " << index << " is out of range" << endl;
        return 0;
    }
}

PlayingArea::Linear * PlayingArea::racket(const qint32 &index) const
{
    if( 0 <= index && index < _rackets.size() )
        return _rackets[index];

    else
    {
        qDebug() << "PlayingArea::racket(i) : given index " << index << " is out of range" << endl;
        return 0;
    }
}

qreal PlayingArea::getWallRotation(const qint32 & wallIndex)
{
    if( 0 <= wallIndex && wallIndex < _walls.size() )
        return _walls[wallIndex]->rotation();

    else
    {
        qDebug() << "PlayingArea::getWallRotation(i) : given index " << wallIndex << " is out of range" << endl;
        return 0;
    }
}

bool PlayingArea::collisionHappened()
{
    bool collision = false;

    collision = _collisionIn(_rackets);

    if(!collision)
        collision = _collisionIn(_cages);

    if(!collision)
        collision = _collisionIn(_walls);

    return collision;
}

bool PlayingArea::_collisionIn(QVector<PlayingArea::Linear *> & items)
{
    QLineF topSide( _ball.rect().topLeft(), _ball.rect().topRight() );
    QLineF rightSide( _ball.rect().topRight(), _ball.rect().bottomRight() );
    QLineF downSide( _ball.rect().bottomRight(), _ball.rect().bottomLeft() );
    QLineF leftSide( _ball.rect().bottomLeft(), _ball.rect().topLeft() );
    QPointF p1, p2;
    bool collision;
    int i;

    i=0;
    collision = false;
    while(i < items.size() && !collision)
    {
        p1 = _ball.mapFromItem( items[i], items[i]->line().p1() );
        p2 = _ball.mapFromItem( items[i], items[i]->line().p2() );
        QLineF itemLine(p1, p2);

        collision = (
                    itemLine.intersect(topSide, 0) == QLineF::BoundedIntersection
                     ||
                     itemLine.intersect(rightSide,0) == QLineF::BoundedIntersection
                     ||
                     itemLine.intersect(downSide,0) == QLineF::BoundedIntersection
                     ||
                     itemLine.intersect(leftSide,0) == QLineF::BoundedIntersection
                    );

        if(collision)
            _ballCollider = items[i];

        else
            ++i;
    }

    return collision;
}

PlayingArea::Linear * PlayingArea::getBallCollider() const
{
    return _ballCollider;
}

void PlayingArea::rebuild(const qint32 &nbPlayers,
                        const qreal &renderAreaWidth,
                        const qreal & ballRadius)
{
    _clear();

    _nbPlayers = nbPlayers;
    _renderAreaWidth = renderAreaWidth;

    setBallRadius(ballRadius);
    resetBallPos();
    build();
}

void PlayingArea::setBallRadius(const qreal &ballRadius)
{
    _ballRadius = ballRadius;
    _ball.setRect(-_ballRadius, -_ballRadius, 2*_ballRadius, 2*_ballRadius);
}

void PlayingArea::resetBallPos()
{
    QPointF pt = _ball.mapFromParent(0,0);
    _ball.setPos(pt);
    _set_ball_random_direction();
}

void PlayingArea::rotateBallDirection(const qreal & alpha)
{
    _ballDirection.setAngle(_ballDirection.angle()+alpha);
}

void PlayingArea::mirrorBallDirection(PlayingArea::Linear * axis)
{
    qreal alpha = _ballDirection.angleTo( axis->line() );

    if(alpha > 180)
        alpha = 360 - alpha;

    if(alpha > 90)
        alpha = 180 - alpha;

    _ballDirection.setAngle(_ballDirection.angle() + 2*alpha);
}

void PlayingArea::moveBall(const qreal & dx, const qreal & dy)
{
    _ball.moveBy(dx, dy);
    _ballDirection.translate( QPointF(dx,dy) );
}

void PlayingArea::moveBall()
{
    moveBall( _ballDirection.dx(), _ballDirection.dy() );
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
        qDebug() << "PlayingArea::moveRacket : given racket index " << racketIndex << " is out of bounds" << endl;
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
    if( 0 <= cageIndex && cageIndex < _cages.size() )
    {

        QPointF p1 = _cages[cageIndex]->line().p1();
        QPointF p2 = _cages[cageIndex]->line().p2();

        p1 = _cages[cageIndex]->mapToParent(p1);
        p2 = _cages[cageIndex]->mapToParent(p2);

        _walls.push_back( new QGraphicsLineItem(QLineF(p1, p2), &_scene) );

        delete _cages[cageIndex];
        _cages.erase(_cages.begin()+cageIndex);
    }

    else
        qDebug() << "PlayingArea::removeCage(i) : given index " << cageIndex << " is out of bounds" << endl;
}

void PlayingArea::removeRacket(const qint32 & racketIndex)
{
    if( 0 <= racketIndex && racketIndex < _rackets.size() )
    {
        delete _rackets[racketIndex];
        _rackets.erase(_rackets.begin()+racketIndex);
    }

    else
        qDebug() << "PlayingArea::removeRacket(i) : given index " << racketIndex << " is out of bounds" << endl;
}

void PlayingArea::removeWall(const qint32 & wallIndex)
{
    if( 0 <= wallIndex && wallIndex < _walls.size() && _nbPlayers >= 3)
    {
        _nbPlayers -= 1;

        _clear();
        resetBallPos();
        build();
    }

    else
        qDebug() << "PlayingArea::removeWall(i) : given index " << wallIndex << " is out of bounds" << endl;
}

void PlayingArea::_clear()
{
    for(qint32 i=0; i<_walls.size(); ++i)
        delete _walls[i];

    for(qint32 i=0; i<_rackets.size(); ++i)
        delete _rackets[i];

    for(qint32 i=0; i<_cages.size(); ++i)
        delete _cages[i];

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
        _walls.push_back( new QGraphicsLineItem(0.,altitude, -wallLength,altitude, &_scene) );
        _walls.push_back( new QGraphicsLineItem(0.,altitude, -wallLength,altitude, &_scene) );
        _rackets.push_back( new QGraphicsLineItem(racketLength/2,altitude, -racketLength/2,altitude, &_scene) );
        _cages.push_back( new QGraphicsLineItem(0.,altitude, -cageLength,altitude, &_scene) );

        dx = sideLength / 2;
        dy = 0;
        _walls[j]->moveBy(dx, dy);
        _walls[j]->setTransformOriginPoint(_walls[j]->mapFromParent(0,0));
        _walls[j]->setRotation(_walls[j]->rotation() + i*alpha);
        ++j;

        dx -= wallLength;
        _cages[i]->moveBy(dx, dy);
        _cages[i]->setTransformOriginPoint(_cages[i]->mapFromParent(0,0));
        _cages[i]->setRotation(_cages[i]->rotation() + i*alpha);

        dy = -_racketToWallSpace;
        _rackets[i]->moveBy(0, dy);
        _rackets[i]->setTransformOriginPoint(_rackets[i]->mapFromParent(0,0));
        _rackets[i]->setRotation(_rackets[i]->rotation() + i*alpha);

        dy = 0;
        dx -= cageLength;
        _walls[j]->moveBy(dx, dy);
        _walls[j]->setTransformOriginPoint(_walls[j]->mapFromParent(0,0));
        _walls[j]->setRotation(_walls[j]->rotation() + i*alpha);

        ++j;
    }
}


void PlayingArea::_set_ball_random_direction()
{
    qreal alpha = 30;//_rndGen.randomIntbeetween(0, 360);

    _ballDirection.setAngle(_ballDirection.angle()+alpha);
}
