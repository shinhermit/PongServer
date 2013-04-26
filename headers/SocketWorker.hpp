#ifndef _SocketWorker
#define _SocketWorker

#include <QObject>
#include <QVector>
#include <QTcpSocket>
#include <QMutex>
#include <QDataStream>
#include <QDebug>
#include <QErrorMessage>
#include <QTimer>

#include "Concurrent.hpp"
#include "PongShared.hpp"
#include "PongServerView.hpp"

class SocketWorker : public Concurrent
{
    Q_OBJECT
public:
    SocketWorker(QTcpSocket * socket,
                 const qint32 & playerIndex,
                 QObject * parent=0);

    ~SocketWorker();

    void operator>>(QDataStream & out);

    void operator<<(QDataStream & in);

    friend QDataStream & operator<<(QDataStream & out, SocketWorker & sckw);
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
    qint32 _playerIndex;
    QTcpSocket * _socket;
    bool _disconnected;

    bool _running_state();
    bool _active_player();
};

#endif
