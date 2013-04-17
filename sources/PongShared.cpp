#include "PongShared.hpp"

short PongShared::maxPlayers = 6;
GameState PongShared::gameState;
PlayingArea PongShared::playingArea;
QVector<PlayerState> PongShared::playersStates;
QMutex PongShared::playersStatesMutex;

