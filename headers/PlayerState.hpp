#ifndef _PlayerState
#define _PlayerState

#include <cmath>

#include <QObject>
#include <QLineF>

#include "PongTypes.hpp"
#include "Lockable.hpp"

/*!
 * \brief The PlayerState class holds the shared state of a player
 */
class PlayerState : public QObject, public Lockable
{
    Q_OBJECT
public:
    /*!
     * \brief Constructor
     * \param id
     * \param state initial state of the player
     * \param parent passed to the superclass QObject
     */
    PlayerState(
            const qint32 & id=0,
            const PongTypes::E_PlayerState & state=PongTypes::WAITING,
            QObject * parent=0
            );

    /*!
     * \brief Copy constructor
     * \param source another PlayerState object
     */
    PlayerState(const PlayerState & source);

    /*!
     * \brief operator =
     */
    PlayerState & operator=(const PlayerState & source);

    /*!
     * \brief operator ==
     */
    bool operator==(const PlayerState & ref)const;

    /*!
     * \brief Sets the id of the player
     *This Id is useful to the communication with the clients.
     * \param id new value
     */
    void setId(const qint32 & id);

    /*!
     * \brief Sets the remaining credit of a player
     *When the credits falls to zero, the player is discarded
     * \param credit new credit
     */
    void setCredit(const qint32 &credit);

    /*!
     * \brief convenience method to decrease the credit
     * \param step how much to decrease
     */
    void decreaseCredit(const qint32 & step=1);

    /*!
     * \brief Sets the state of the player
     * \param state new state
     */
    void setState(const PongTypes::E_PlayerState & state);

    /*!
     * \brief Convenience method to set the state of the player to discarded
     */
    void setFailState();

    /*!
     * \brief Sets the line of the racket of the player
     * \param racket new base line of the racket
     */
    void setRacket(const QLineF & racket);

    /*!
     * \brief Convenience method to set the base line of the racket of a player
     *This method is equivalent to setRacket(QLineF(p1, p2))
     * \param p1 first point of the racket
     * \param p2 second point of the racket
     */
    void setRacket(const QPointF & p1, const QPointF & p2);

    /*!
     * \brief Convenience method to set the base line of the racket of a player
     * \param x1 x coord of the first point of the racket's line
     * \param y1 y coord of the first point of the racket's line
     * \param x2 x coord of the second point of the racket's line
     * \param y2 y coord of the second point of the racket's line
     */
    void setRacket(const qreal & x1,
                   const qreal & y1,
                   const qreal & x2,
                   const qreal & y2);

    /*!
     * \brief Gets the id of the player
     * \return id of the players
     */
    const qint32 & id()const;

    /*!
     * \brief Gets the base line of the racket of the player
     * \return racket's base line
     */
    const QLineF & racket()const;

    /*!
     * \brief Gets the remaining credit of the player
     * \return remaining credit
     */
    const qint32 & credit()const;

    /*!
     * \brief Gets the current state of the player
     * \return player's state
     */
    const PongTypes::E_PlayerState & state()const;

    /*!
     * \brief Gets the default inital credit of a player
     * \return default initial credit
     */
    static const qint32 &DefaultCredit();

private:
    qint32 _myIndex; /*!< index of the player */
    QLineF _racket; /*!<  base line of the player's racket*/
    qint32 _credit; /*!< remaining credit */
    PongTypes::E_PlayerState _state; /*!< current state of the player */

    static const qint32 _defaultCredit; /*!< default initial credit */
};

#endif
