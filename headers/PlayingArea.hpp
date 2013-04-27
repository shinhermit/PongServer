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

/*!
 * \brief The PlayingArea class holds the game arena
 *It is intended to be shared amoung Concurrent objects through the PongShared class.
 */
class PlayingArea : public Lockable
{
public:
    /*!
     * \brief type definition for evolutivity
     *Represents every linear element in the playing area. These are actually everything except the ball
     */
    typedef QGraphicsLineItem Linear;

    /*!
     * \brief type definition for evolutivity
     *Represents the ball
     */
    typedef QGraphicsEllipseItem Ball;

    /*!
     * \brief type definition for evolutivity
     *Represents the bounding frame of the playing arena
     */
    typedef QGraphicsRectItem Scene;

    /*!
     * \brief Constructor
     * \param nbPlayers initial number of player. Useful to setup the initial shape of the arena.
     * \param renderAreaWidth width of the bounding square of the area
     * \param ballRadius radius of the ball
     */
    PlayingArea(const qint32 & nbPlayers=6,
                const qint32 & renderAreaWidth=600,
                const qreal & ballRadius=5);

    /*!
     * \brief Builds the arena
     *Sets the shape and the elements of the arena according to the number of players. With 3 players, it will be a triangle shape, 4 players a square, etc.
     */
    void build();

    /*!
     * \brief Gets the position of the center of the playing area
     * \return playing  area's center point
     */
    QPointF centerPoint()const;

    /*!
     * \brief Gets the center angle to the any side of the playing arena
     * \return playing arena's sides center angle
     */
    qreal centerAngle()const;

    /*!
     * \brief Gets the width of the side of the playing arena
     * \return playing arena's side width
     */
    qreal areaWidth()const;

    /*!
     * \brief Gets the bounding frame of the playing are
     * \return playing area's bounding frame
     */
    Scene * scene();

    /*!
     * \brief Tells if the given PlayingArea::Linear object is a cage
     * \param item tested PlayingArea::Linear object
     * \return true if item is a cage, false elsewise
     */
    bool isCage(Linear *item)const;

    /*!
     * \brief Tells if the given PlayingArea::Linear object is a racket
     * \param item tested PlayingArea::Linear object
     * \return true if item is a racket, false elsewise
     */
    bool isRacket(Linear *item)const;

    /*!
     * \brief Tells if the given PlayingArea::Linear object is a wall
     * \param item tested PlayingArea::Linear object
     * \return true if item is a wall, false elsewise
     */
    bool isWall(Linear *item)const;

    /*!
     * \brief Gets the numbers of the players know by the area
     * \return playing area's number of players
     */
    qint32 nbPlayers()const;

    /*!
     * \brief Gets the numbers of cages in the area
     * \return
     */
    qint32 nbCages()const;

    /*!
     * \brief Gets the number of rackets in the are
     * \return
     */
    qint32 nbRackets()const;

    /*!
     * \brief gets the number of walls in the area
     * \return
     */
    qint32 nbWalls()const;

    /*!
     * \brief Gets the position of the ball
     * \return
     */
    QPointF ballPos()const;

    /*!
     * \brief gets the bounding rectangle of the ball
     * \return
     */
    QRectF ballRect()const;

    /*!
     * \brief Gets the index of the given cage
     * \param item tested PlayingArea::Linear object
     * \return -1 if item is not found amoung playing area's cages, the index of item elsewise
     */
    qint32 cageIndex(Linear *item)const;

    /*!
     * \brief Gets the index of the given racket
     * \param item tested PlayingArea::Linear object
     * \return -1 if item is not found amoung playing area's rackets, the index of item elsewise
     */
    qint32 racketIndex(Linear * item)const;

    /*!
     * \brief Gets the index of the given wall
     * \param item tested PlayingArea::Linear object
     * \return -1 if item is not found amoung playing area's walls, the index of item elsewise
     */
    qint32 wallIndex(Linear * item)const;

    /*!
     * \brief Gets the wall indiced by the given index
     * \param index indice of the looked up wall
     * \return 0 if index is out of bounds, a pointer to the corresponding PlayingArea::Linear object elsewise
     */
    Linear * wall(const qint32 & index) const;

    /*!
     * \brief Gets the cage indiced by the given index
     * \param index indice of the looked up wall
     * \return 0 if index is out of bounds, a pointer to the corresponding PlayingArea::Linear object elsewise
     */
    Linear * cage(const qint32 & index) const;

    /*!
     * \brief Gets the racket indiced by the given index
     * \param index indice of the looked up wall
     * \return 0 if index is out of bounds, a pointer to the corresponding PlayingArea::Linear object elsewise
     */
    Linear * racket(const qint32 & index) const;

    /*!
     * \brief Gets the base line of the racket with the given index
     * \param index indice of the racket of which we want the base line
     * \return base line of the racket
     */
    QLineF racketLine(const qint32 & index)const;

    /*!
     * \brief Gets the rotation angle of the wall with the given index
     * \param wallIndex indice of the wall of which we want the rotation angle
     * \return rotation angle of the wall
     */
    qreal getWallRotation(const qint32 & wallIndex);

    /*!
     * \brief Tells if a collision happenned
     * \return true if the ball collided any PlayingArea::Linear object in the area, false elsewise
     */
    bool collisionHappened();

    /*!
     * \brief Gets the PlayingArea::Linear object that last collided with the ball
     * \return ball colliding PlayingArea::Linear object
     */
    Linear * getBallCollider()const;

    /*!
     * \brief builds the playing area with the given parameters
     * \param nbPlayers number of players; defines the shape of the arena
     * \param renderAreaWidth width of the bounding frame
     * \param ballRadius radius of the ball, that the width of its bounding square
     */
    void rebuild(const qint32 & nbPlayers=6,
                 const qreal & renderAreaWidth=600.,
                 const qreal & ballRadius=5);
    /*!
     * \brief Sets the radius of the ball, that is its bounding square width
     * \param ballRadius new value of the radius
     */
    void setBallRadius(const qreal & ballRadius);

    /*!
     * \brief Set the postion of the ball at the center of the PlayingArea::Scene object
     */
    void resetBallPos();

    /*!
     * \brief changes the direction of the (moving) ball
     * \param alpha new angle for the direction of the ball
     */
    void rotateBallDirection(const qreal & alpha);

    /*!
     * \brief Mirrors the ball moving vector using the given axis
     * \param axis mirror axis
     */
    void mirrorBallDirection(Linear * axis);

    /*!
     * \brief Translates the ball by the given delta values
     * \param dx delta x translate value
     * \param dy delta y translate value
     */
    void moveBall(const qreal & dx, const qreal &dy);

    /*!
     * \brief Translates the ball by the default delta values.
     *The default translate delta value is defined by private PlayingArea::_ballTranslateQuantum
     */
    void moveBall();

    /*!
     * \brief Translates the racket on x axis in local coordinate
     * \param racketIndex index of the racket we want to move
     * \param delta length the X axis tranlation
     */
    void moveRacket(const qint32 &racketIndex,
                    const qreal & delta);

    /*!
     * \brief Sets the base line of a racket
     * \param racketIndex index of the racket we want to reset
     * \param line new base line
     */
    void setRacketLine(const qint32 &racketIndex,
                       const QLineF & line);

    /*!
     * \brief Sets the X coordinate of the position a racket
     * \param racketIndex index of the racket we want to transform
     * \param abss new abscisse
     */
    void setRacketAbss(const qint32 & racketIndex,
                       const qreal & abss);

    /*!
     * \brief Sets the Y coordinate of the position a racket
     * \param racketIndex index of the racket we want to transform
     * \param abss new ordinate
     */
    void setRacketOrd(const qint32 & racketIndex,
                      const qreal & ord);

    /*!
     * \brief Sets the coordinates of a racket
     * \param racketIndex the racket we want to move
     * \param x new abscisse
     * \param y new ordinate
     */
    void setRacketCoord(const qint32 & racketIndex,
                        const qreal & x,
                        const qreal & y);

    /*!
     * \brief Turns a cage into a wall
     * \param cageIndex index of the cage we want to convert
     */
    void removeCage(const qint32 & cageIndex);

    /*!
     * \brief removes a racket from the area
     * \param racketIndex index of the racket we want to remove
     */
    void removeRacket(const qint32 & racketIndex);

    /*!
     * \brief Removes a wall from the playing area
     *This action implies an implicit rebuild of the area
     * \param wallIndex index of the wall we want to remove.
     */
    void removeWall(const qint32 & wallIndex);

private:
    qint32 _nbPlayers; /*!< the number of players on which the automatic generation is based*/
    qint32 _renderAreaWidth; /*!< the width of the arena bounding frame*/
    qreal _centerAngle; /*!< the center angle to any side of the playing area*/
    MathJ::Random _rndGen; /*!< random number generator */

    Scene _scene; /*!< the bounding frame of the arena*/
    qreal _ballRadius; /*!< the radius of the ball, that is the width of its bounding square*/
    Ball _ball; /*!< the ball*/
    QLineF _ballDirection; /*!< the directing vector of the ball*/
    QVector<Linear*> _cages;/*!< cages of the arena*/
    QVector<Linear*> _rackets;/*!< rackets arena*/
    QVector<Linear*> _walls;/*!< walls the arena*/

    Linear * _ballCollider;/*!< the last PlayingArea::Linear object that collided with the ball*/

    static const qreal _wallRatio;/*!< ratio of any wall to the side of the arena*/
    static const qreal _cageRatio;/*!< ratio of any cage to the side of the arena*/
    static const qreal _racketRatio;/*!< ratio of any racket to the side of the arena*/
    static const qreal _racketToWallSpace;/*!< distance between a racket a the side of arena*/
    static const qreal _ballTranslateQuantum;/*!< minimal distance of the ball translation*/

    /*!
     * \brief Removes every elements of the playing area
     */
    void _clear();

    /*!
     * \brief automatic areana generator
     *This generator can by used only when nbPlayers is greater or equal to 3. This is the reason of its privacy. The general generator is PlayingArea::build()
     * \param nbPlayers
     */
    void _generate_area(const qint32 & nbPlayers);

    /*!
     * \brief Performs translation and rotation onto lines.
     *This method helps for the automatic generation of the playing area.
     * \param line the line to transform
     * \param dx X axis delta translation
     * \param dy Y axis delta translation
     * \param alpha rotation angle
     */
    void _set_line_position(QLineF & line,
                            const qreal & dx,
                            const qreal & dy,
                            const qreal & alpha);

    /*!
     * \brief Sets a random angle to the ball directing vector
     */
    void _set_ball_random_direction();

    /*!
     * \brief Tells if a collision happened in a set of PlayingArea::Linear objects
     * \param items a vector of items tested for collision with the ball
     * \return
     */
    bool _collisionIn(QVector<Linear*> & items);
};

#endif
