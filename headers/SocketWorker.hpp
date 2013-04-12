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
    SocketWorker(QObject &parent,
                 qint32 myIndex,
                 PongServerView &view,
                 QTcpSocket &socket,
                 PlayingArea &playingArea,
                 GameState &gameState,
                 QVector<PlayerState *> &playersStates
                 );

    SocketWorker(qint32 myIndex,
                 PongServerView & view,
                 QTcpSocket & socket,
                 PlayingArea & playingArea,
                 GameState & gameState,
                 QVector<PlayerState*> & playersStates
                 );

    void operator>>(QDataStream & out)const;

    void operator<<(QDataStream & in);

    friend QDataStream & operator<<(QDataStream & out, const SocketWorker & sckw);
    friend QDataStream & operator>>(QDataStream & in, SocketWorker & sckw);

    void setId(const qint32 & index);
    const qint32 & id()const;

signals:
    void hostDisconnected();
    void finishedSignal();

public slots:
    void beginInteract();
    void socketError( QAbstractSocket::SocketError socketError );
    void disconnected();
    void quitSlot();

private:
    PongServerView & _view;
    QTcpSocket & _socket;
    PlayingArea & _playingArea;
    GameState & _gameState;
    QVector<PlayerState*> & _playersStates;

    QDataStream _socket_stream;
    qint32 _myIndex;

    bool _running_state();
};

#endif
