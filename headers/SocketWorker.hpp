#ifndef _SocketWorker
#define _SocketWorker

#include <QObject>
#include <QVector>
#include <QTcpSocket>
#include <QMutex>
#include <QDataStream>
#include<QDebug>

#include "PlayerState.hpp"
#include "PlayingArea.hpp"
#include "GameState.hpp"
#include "PongServerView.hpp"

class SocketWorker : public QObject
{
    Q_OBJECT
public:
    SocketWorker(
            PongServerView &view,
            QTcpSocket &socket,
            PlayingArea &playingArea,
            GameState &gameState,
            PlayerState &playerState
            );

    ~SocketWorker();

    void operator>>(QDataStream & out)const;

    void operator<<(QDataStream & in);

    friend QDataStream & operator<<(QDataStream & out, const SocketWorker & sckw);
    friend QDataStream & operator>>(QDataStream & in, SocketWorker & sckw);

signals:
    void hostDisconnected(); //stop thread
    void finishedSignal();

public slots:
    void beginInteract();
    void socketError( QAbstractSocket::SocketError socketError );
    void disconnected();

private:
    QDataStream _socket_stream;

    PongServerView & _view;
    QTcpSocket & _socket;
    PlayingArea & _playingArea;
    GameState & _gameState;
    PlayerState & _playerState;

    bool _running_state();
    bool _exit_requested();
};

#endif
