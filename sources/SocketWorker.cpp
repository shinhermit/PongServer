#include "SocketWorker.hpp"

SocketWorker::SocketWorker(
        QObject &parent,
        QTcpSocket &socket,
        PlayingArea &playingArea,
        GameState &gameState,
        QVector<PlayerState *> &playersStates
        ):
    QObject(&parent),
    _socket(socket),
    _playingArea(playingArea),
    _gameState(gameState),
    _playersStates(playersStates)

{}


SocketWorker::SocketWorker(QTcpSocket &socket,
                           PlayingArea &playingArea,
                           GameState &gameState,
                           QVector<PlayerState *> &playersStates
                           ):
    _socket(socket),
    _playingArea(playingArea),
    _gameState(gameState),
    _playersStates(playersStates)
{}


void SocketWorker::beginInteract()
{
}
