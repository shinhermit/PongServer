#include "PlayingArea.hpp"

const qreal PlayingArea::_wallRatio = 1./4;
const qreal PlayingArea::_cageRatio = 1./2;
const qreal PlayingArea::_racketRatio = 1./6;
const qreal PlayingArea::_racketToWallSpace = 20;
const qreal PlayingArea::_penWidth = 3;

void PlayingArea::_generate_area()
{
    qreal dx=0., dy=0.;

    qreal radius = _renderAreaWidth / 2;
    qreal beta = (180 - _centerAngle) / 2;

    qreal sideLength = radius * ( ::sin(MathUtils::DegreeToRadian(_centerAngle)) / ::sin(MathUtils::DegreeToRadian(beta)) );
    qreal wallLength = sideLength * _wallRatio;
    qreal racketLength = sideLength * _racketRatio;
    qreal cageLength = sideLength * _cageRatio;
    qreal altitude = radius * ::cos( MathUtils::DegreeToRadian(_centerAngle/2) );

    _walls.push_back( new QGraphicsLineItem(0.,altitude, -wallLength,altitude, 0, _scene) );
    _walls.push_back( new QGraphicsLineItem(0.,altitude, -wallLength,altitude, 0, _scene) );
    _rackets.push_back( new QGraphicsLineItem(racketLength/2,altitude, -racketLength/2,altitude, 0, _scene) );
    _cages.push_back( new QGraphicsLineItem(0.,altitude, -cageLength,altitude, 0, _scene) );

    //tests
    _scene->addEllipse(-radius,-radius,2*radius, 2*radius);
    _scene->addLine(0,0,0, altitude);
    _scene->addLine(-sideLength/2,-altitude, sideLength/2, -altitude);
    _scene->addText(QString::number(_centerAngle)+" radius: "+QString::number(radius)+" sideLength: "+QString::number(sideLength));

    dx = sideLength / 2;
    _walls[0]->moveBy(dx, dy);
    _walls[0]->setPen(QPen(Qt::blue, _penWidth, Qt::SolidLine));

    dx -= wallLength - _penWidth;
    _cages[0]->moveBy(dx, dy);

    dy = -_racketToWallSpace;
    _rackets[0]->moveBy(0, dy);
    _rackets[0]->setPen( QPen(Qt::darkRed, _penWidth, Qt::SolidLine) );

    dy = 0;
    dx -= cageLength - _penWidth;
    _walls[1]->moveBy(dx, dy);
    _walls[1]->setPen(QPen(Qt::blue, _penWidth, Qt::SolidLine));


    for(int i=1; i < _nbPlayers; ++i)
    {
        QGraphicsLineItem * wall1 = new QGraphicsLineItem(_walls[0]->line(), 0, _scene);
        QGraphicsLineItem * wall2 = new QGraphicsLineItem(_walls[1]->line(), 0, _scene);
        QGraphicsLineItem * cage = new QGraphicsLineItem(_cages[0]->line(), 0, _scene);
        QGraphicsLineItem * racket = new QGraphicsLineItem(_rackets[0]->line(), 0, _scene);

        wall1->setTransformOriginPoint(wall1->mapFromScene(0,0));
        wall1->setRotation(i*_centerAngle);
        wall1->setPen(QPen(Qt::blue, _penWidth, Qt::SolidLine));

        wall2->setTransformOriginPoint(wall1->mapFromScene(0,0));
        wall2->setRotation(i*_centerAngle);
        wall2->setPen(QPen(Qt::blue, _penWidth, Qt::SolidLine));

        cage->setTransformOriginPoint(cage->mapFromScene(0,0));
        cage->setRotation(i*_centerAngle);

        racket->setTransformOriginPoint(racket->mapToScene(0,0));
        racket->setRotation(i*_centerAngle);
        _rackets[0]->setPen( QPen(Qt::darkRed, _penWidth, Qt::SolidLine) );

        _walls.push_back(wall1);
        _walls.push_back(wall2);
        _cages.push_back(cage);
        _rackets.push_back(racket);
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
