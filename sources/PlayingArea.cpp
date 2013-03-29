#include "PlayingArea.hpp"

const qreal PlayingArea::_wallRatio = 1./4;
const qreal PlayingArea::_cageRatio = 1./2;
const qreal PlayingArea::_racketRatio = 1./6;
const qreal PlayingArea::_racketToWallSpace = 20;
const qreal PlayingArea::_penWidth = 3;

void PlayingArea::_generate_area()
{
    int j;
    qreal dx, dy;
    qreal radius = _renderAreaWidth / 2;
    qreal beta = (180 - _centerAngle) / 2;

    qreal sideLength = radius * ( ::sin(MathUtils::DegreeToRadian(_centerAngle)) / ::sin(MathUtils::DegreeToRadian(beta)) );
    qreal wallLength = sideLength * _wallRatio;
    qreal racketLength = sideLength * _racketRatio;
    qreal cageLength = sideLength * _cageRatio;
    qreal altitude = radius * ::cos( MathUtils::DegreeToRadian(_centerAngle/2) );

//    //tests
//    _scene->addEllipse(-radius,-radius,2*radius, 2*radius);
//    _scene->addLine(0,0,0, altitude);
//    _scene->addLine(-sideLength/2,-altitude, sideLength/2, -altitude);
//    _scene->addText(QString("alpha degre: ")+QString::number(_centerAngle)+
//                    "\nalpha radian: "+QString::number(MathUtils::DegreeToRadian(_centerAngle))+
//                    "\nradius: "+QString::number(radius)+
//                    "\nbeta (Pi/6="+QString::number(MathUtils::Pi()/6)+"): "+QString::number(MathUtils::DegreeToRadian(beta))+
//                    "\nx: "+QString::number(sideLength)+
//                    "\nh: "+QString::number(altitude)

//                    );

    j = 0;
    for(int i=0; i < _nbPlayers; ++i)
    {
         _walls.push_back( new QGraphicsLineItem(0.,altitude, -wallLength,altitude, 0, _scene) );
        _walls.push_back( new QGraphicsLineItem(0.,altitude, -wallLength,altitude, 0, _scene) );
        _rackets.push_back( new QGraphicsLineItem(racketLength/2,altitude, -racketLength/2,altitude, 0, _scene) );
        _cages.push_back( new QGraphicsLineItem(0.,altitude, -cageLength,altitude, 0, _scene) );

        dx = sideLength / 2;
        dy = 0;
        _walls[j]->moveBy(dx, dy);
        _walls[j]->setPen(QPen(Qt::blue, _penWidth, Qt::SolidLine));
        _walls[j]->setTransformOriginPoint(_walls[j]->mapFromScene(0,0));
        _walls[j]->setRotation(_walls[j]->rotation() + i*_centerAngle);
        ++j;

        dx -= wallLength - _penWidth;
        _cages[i]->moveBy(dx, dy);
        _cages[i]->setTransformOriginPoint(_cages[i]->mapFromScene(0,0));
        _cages[i]->setRotation(_cages[i]->rotation() + i*_centerAngle);

        dy = -_racketToWallSpace;
        _rackets[i]->moveBy(0, dy);
        _rackets[i]->setPen( QPen(Qt::darkRed, _penWidth, Qt::SolidLine) );
        _rackets[i]->setTransformOriginPoint(_rackets[i]->mapFromScene(0,0));
        _rackets[i]->setRotation(_rackets[i]->rotation() + i*_centerAngle);

        dy = 0;
        dx -= cageLength - _penWidth;
        _walls[j]->moveBy(dx, dy);
        _walls[j]->setPen(QPen(Qt::blue, _penWidth, Qt::SolidLine));
        _walls[j]->setTransformOriginPoint(_walls[j]->mapFromScene(0,0));
        _walls[j]->setRotation(_walls[j]->rotation() + i*_centerAngle);

        ++j;
    }
}

PlayingArea::PlayingArea(const int & nbPlayers,
                         const qreal & renderAreaWidth,
                         const QRectF & ball)
    :_nbPlayers(nbPlayers),
      _renderAreaWidth(renderAreaWidth),
      _centerAngle(360/nbPlayers),
      _scene(new QGraphicsScene( QRectF( -renderAreaWidth/2, -renderAreaWidth/2, renderAreaWidth, renderAreaWidth ) ) )
{
    _ball = new QGraphicsRectItem(ball, 0, _scene);
    //_ball->setPen( QPen( Qt::red, 3, Qt::SolidLine) );
    _ball->setBrush( Qt::red );
    _generate_area();
}

PlayingArea::PlayingArea(const int & nbPlayers,
                         const qreal & renderAreaWidth)
    :_nbPlayers(nbPlayers),
      _renderAreaWidth(renderAreaWidth),
      _centerAngle(360/nbPlayers),
      _scene(new QGraphicsScene( QRectF( -renderAreaWidth/2, -renderAreaWidth/2, renderAreaWidth, renderAreaWidth) ) )
{
    _ball = new QGraphicsRectItem(QRectF( 0, 0, 10, 10 ), 0, _scene);
    //_ball->setPen( QPen( Qt::red, 3, Qt::SolidLine) );
    _ball->setBrush( Qt::red );
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

bool PlayingArea::isCage(const QGraphicsItem & item)const
{
    return false;
}

bool PlayingArea::isRacket(const QGraphicsItem & item)const
{
    return false;
}

bool PlayingArea::isWall(const QGraphicsItem & item)const
{
    return false;
}

int PlayingArea::cageIndex(const QGraphicsItem & item)const
{
    return -1;
}

int PlayingArea::racketIndex(const QGraphicsItem & item)const
{
    return -1;
}

int PlayingArea::wallIndex(const QGraphicsItem & item)const
{
    return 0;
}

qreal PlayingArea::getWallRotation(const int & wallIndex)
{
    return 0.;
}

void PlayingArea::update()
{
}

void PlayingArea::reset(const int &nbPlayers,
                        const qreal &renderAreaWidth,
                        const QRectF &ball)
{
}

void PlayingArea::reset(const int &nbPlayers,
                        const qreal &renderAreaWidth)
{
}

void PlayingArea::resetBall()
{
}

void PlayingArea::rotateBallDirection(const qreal & alpha)
{
}

void PlayingArea::mirrorBallDirection(const qreal & m11,
                                      const qreal & m12,
                                      const qreal & m21,
                                      const qreal & m22)
{
}

void PlayingArea::moveBall(const qreal & delta)
{
}

QList<QGraphicsItem*> PlayingArea::getBallColliders()const
{
    return QList<QGraphicsItem*>();
}

void PlayingArea::setRacketAbss(const int & racketIndex,
                                const qreal & abss)
{
}

void PlayingArea::setRacketOrd(const int & racketIndex,
                               const qreal & ord)
{
}

void PlayingArea::setRacketCoord(const int & racketIndex,
                                 const QPointF & p1,
                                 const QPointF & p2)
{
}

void PlayingArea::removeCage(const int & cageIndex)
{
}

void PlayingArea::removeRacket(const int & racketIndex)
{
}

void PlayingArea::removeWall(const int & wallIndex)
{
}
