#ifndef CONCURRENT_HPP
#define CONCURRENT_HPP

#include <QString>
#include <QDebug>

#include "PongShared.hpp"

class Concurrent
{
public:
    Concurrent();

    void lockGameState();
    void lockPlayingArea();
    void lockPlayersStates();

    void unlockGameState();
    void unlockPlayingArea();
    void unlockPlayersStates();

private:
    bool _I_locked_gameState;
    bool _I_locked_playingArea;
    bool _I_locked_playersStates;
    QString _lockerName;//debug
};

#endif // CONCURRENT_HPP
