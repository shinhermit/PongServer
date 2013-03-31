#ifndef _SocketWorker
#define _SocketWorker

#include <QObject>
#include <QVector>
#include <QTcpSocket>
#include <QMutex>

#include "PlayerState.hpp"
#include "PlayingArea.hpp"
#include "GameState.hpp"

class SocketWorker : public QObject
{
    Q_OBJECT
public:
    SocketWorker(
            QObject & parent,
            QTcpSocket & socket,
            PlayingArea & playingArea,
            GameState & gameState,
            QVector<PlayerState*> & playersStates
            );

    SocketWorker(
            QTcpSocket & socket,
            PlayingArea & playingArea,
            GameState & gameState,
            QVector<PlayerState*> & playersStates
            );

public slots:
    void beginInteract();

private:
    QTcpSocket & _socket;
    PlayingArea & _playingArea;
    GameState & _gameState;
    QVector<PlayerState*> & _playersStates;
};

#endif
