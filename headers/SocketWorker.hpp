#ifndef _SocketWorker
#define _SocketWorker

#include <QObject>
#include <QVector>
#include <QTcpSocket>
#include <QMutex>
#include <QDataStream>
#include <QDebug>
#include <QErrorMessage>

#include "PlayerState.hpp"
#include "PlayingArea.hpp"
#include "GameState.hpp"
#include "PongServerView.hpp"

class SocketWorker : public QObject
{
    Q_OBJECT
public:
    SocketWorker(
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
    void appendStatusSignal(const QString & status);
    void sendDataSignal();

public slots:
    void beginInteract();
    void sendDataSlot();
    void getDataSlot();
    void socketError( QAbstractSocket::SocketError socketError );
    void disconnected();

private:
    QDataStream _streamer;

    QTcpSocket & _socket;
    PlayingArea & _playingArea;
    GameState & _gameState;
    PlayerState & _playerState;

    bool _running_state();
    bool _exit_requested();
};

#endif
