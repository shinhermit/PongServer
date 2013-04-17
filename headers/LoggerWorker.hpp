#ifndef _LoggerWorker
#define _LoggerWorker

#include <cmath>

#include <QObject>
#include <QThread>
#include <QVector>
#include <QMutex>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>

#include "PongShared.hpp"
#include "Concurrent.hpp"
#include "SocketWorker.hpp"
#include "PongServerView.hpp"

class LoggerWorker : public QObject, public Concurrent
{
    Q_OBJECT
public:
    LoggerWorker(const qint16 & port=6666);

    ~LoggerWorker();

    void setlisteningPort(const qint16 & port);

signals:
    void newPlayerConnected();
    void appendStatusSignal(const QString & status);
    void startService();

public slots:
    void waitConnections();
    void newConnectionSlot();
    void appendStatusSlot(const QString & status);

private:
    qint16 _port;
    QTcpServer _tcpServer;

    static const short _maxPending;

    bool _loggableGameState();

    bool _exit_requested();

    void _setNbPlayers(const qint32 & nbPlayers);

    void _incNbPlayers();

    qint32 _nbPlayers();
};

#endif
