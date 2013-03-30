#include "PlayingArea.hpp"

using namespace MathJ;

const qreal PlayingArea::_wallRatio = 1./4;
const qreal PlayingArea::_cageRatio = 1./2;
const qreal PlayingArea::_racketRatio = 1./6;
const qreal PlayingArea::_racketToWallSpace = 20;
const qreal PlayingArea::_penWidth = 5;
const qreal PlayingArea::_ballRadius = 5;
const qreal PlayingArea::_ballTranslateQuantum = 30;

void PlayingArea::_generate_area(const int & nbPlayers)
{
    int j;
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
    for(int i=0; i < nbPlayers; ++i)
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
        _walls[j]->setRotation(_walls[j]->rotation() + i*alpha);
        ++j;

        dx -= wallLength;
        _cages[i]->moveBy(dx, dy);
        _cages[i]->setTransformOriginPoint(_cages[i]->mapFromScene(0,0));
        _cages[i]->setRotation(_cages[i]->rotation() + i*alpha);

        dy = -_racketToWallSpace;
        _rackets[i]->moveBy(0, dy);
        _rackets[i]->setPen( QPen(Qt::darkRed, _penWidth, Qt::SolidLine) );
        _rackets[i]->setTransformOriginPoint(_rackets[i]->mapFromScene(0,0));
        _rackets[i]->setRotation(_rackets[i]->rotation() + i*alpha);

        dy = 0;
        dx -= cageLength;
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
    qreal alpha = _rndGen.randomIntbeetween(-180, 180);

    _ballDirection.setAngle(_ballDirection.angle()+alpha);

    //tests
    _scene->addLine(_ballDirection);
}

PlayingArea::PlayingArea(const int & nbPlayers,
                         const qreal & renderAreaWidth,
                         const QRectF & ball)
    :_nbPlayers(nbPlayers),
      _renderAreaWidth(renderAreaWidth),
      _centerAngle(360/nbPlayers),
      _scene(new QGraphicsScene( QRectF( -renderAreaWidth/2, -renderAreaWidth/2, renderAreaWidth, renderAreaWidth ) ) ),
      _ballDirection(0,0, _ballTranslateQuantum, 0)
{
    _ball = new QGraphicsEllipseItem(ball, 0, _scene);
    _ball->setBrush( Qt::red );
    _set_ball_random_direction();

    _generate_area();
}

PlayingArea::PlayingArea(const int & nbPlayers,
                         const qreal & renderAreaWidth)
    :_nbPlayers(nbPlayers),
      _renderAreaWidth(renderAreaWidth),
      _centerAngle(360/nbPlayers),
      _scene(new QGraphicsScene( QRectF( -renderAreaWidth/2, -renderAreaWidth/2, renderAreaWidth, renderAreaWidth) ) ),
      _ballDirection(0,0, _ballTranslateQuantum, 0)
{
    _ball = new QGraphicsEllipseItem(QRectF( -_ballRadius, -_ballRadius, 2*_ballRadius, 2*_ballRadius ), 0, _scene);
    _ball->setBrush( Qt::red );
    _set_ball_random_direction();

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
    if( 0 <= cageIndex && cageIndex < _cages.size() )
    {

        QPointF p1 = _cages[cageIndex]->line().p1();
        QPointF p2 = _cages[cageIndex]->line().p2();

        p1 = _cages[cageIndex]->mapToScene(p1);
        p2 = _cages[cageIndex]->mapToScene(p2);

        _walls.push_back( new QGraphicsLineItem(QLineF(p1, p2), 0, _scene) );
        _walls[_walls.size()-1]->setPen(QPen(Qt::blue, _penWidth, Qt::SolidLine));

        _scene->removeItem(_cages[cageIndex]);
        _cages.erase(_cages.begin()+cageIndex);
    }
}

void PlayingArea::removeRacket(const int & racketIndex)
{
    if( 0 <= racketIndex && racketIndex < _rackets.size() )
    {
        _scene->removeItem(_rackets[racketIndex]);
        _rackets.erase(_rackets.begin()+racketIndex);
    }
}

void PlayingArea::removeWall(const int & wallIndex)
{
    if( 0 <= wallIndex && wallIndex < _walls.size() && _nbPlayers >= 3)
    {
        _nbPlayers -= 1;
        _generate_area();
    }

}
