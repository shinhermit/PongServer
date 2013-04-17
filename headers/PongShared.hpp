#ifndef PONGSHARED_HPP
#define PONGSHARED_HPP

#include <QMutex>

#include "GameState.hpp"
#include "PlayingArea.hpp"
#include "PlayerState.hpp"

class PongShared
{
public:
    static short maxPlayers;
    static GameState gameState;
    static PlayingArea playingArea;
    static QVector<PlayerState> playersStates;
    //Mutex for vector
    static QMutex playersStatesMutex;

};

#endif // PONGSHARED_HPP
