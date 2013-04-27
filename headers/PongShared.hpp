#ifndef PONGSHARED_HPP
#define PONGSHARED_HPP

#include <QMutex>

#include "GameState.hpp"
#include "PlayingArea.hpp"
#include "PlayerState.hpp"

/*!
 * \brief The PongShared class provides object intended to be shared amoung multiple threads
 */
class PongShared
{
public:
    static short maxPlayers; /*!< The max number of player who can play simultaneously*/
    static GameState gameState; /*!< The shared states of the game*/
    static PlayingArea playingArea; /*!< The share playing area*/
    static QVector<PlayerState> playersStates; /*!< The shared states of the players*/
    //Mutex for vector
    static QMutex playersStatesMutex; /*!< The global mutex for the vector of shared players states*/
};

#endif // PONGSHARED_HPP
