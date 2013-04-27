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

/*!
 * \brief The LoggerWorker class manages the connection of new players
 *The main tasks of the loggerWorker are:
 *. listen for incoming connecttions
 *. create a decated SocketWorker for each player
 *. inform of the new connection
 *. stop connecting players when the max number of player is reached
 */
class LoggerWorker : public Concurrent
{
    Q_OBJECT
public:
    /*!
     * \brief Constructor
     * \param port the port on which the LoggerWorker listens for incoming connection
     */
    LoggerWorker(const qint16 & port=6666);

    ~LoggerWorker();

    /*!
     * \brief Sets the listening port
     * \param port the port number to listen on
     */
    void setlisteningPort(const qint16 & port);

signals:
    /*!
     * \brief signal emitted when a new connection is accepted
     *This signal should be connected inside PongServer to the slot PongServer::newPlayerConnected()
     */
    void newPlayerConnected();

    /*!
     * \brief appends a status message to the PongServer view
     *This signal should be connected inside PongServer to the slot PongServerView::appendStatusSlot()
     * \param status the text message to be appended
     */
    void appendStatusSignal(const QString & status);

    /*!
     * \brief starts the newly created SocketWorker
     */
    void startService();

public slots:
    /*!
     * \brief start listenning for incoming connections
     *This slot should be connected inside PongServer to the signal PongServer::startService().
     */
    void waitConnections();

    /*!
     * \brief manages any incoming connection
     *This slot should be internally connected to the slot QTcpServer::newConnection() of the internal QtcpServer object
     */
    void newConnectionSlot();

    /*!
     * \brief Relays the appendStatusSignal of the SocketWorker created by this LoggerWorker
     * \param status the text message received within the appendStatusSignal of the SocketWorker emitter
     */
    void appendStatusSlot(const QString & status);

private:
    qint16 _port; /*!< port where the internal QTcpServer object listens*/
    QTcpServer _tcpServer; /*!< internal QTcpServer object */

    static const short _maxPending; /*!< defines the max number of pending connections for the server */

    /*!
     * \brief checks if the game is in a state in which LoggerWorker can accept connections
     *States in which LoggerWorker can not accpet new connections include PongTypes::RUNNING, PongTypes::INITIALIZING and PongTypes::EXIT_REQUESTED
     * \return true if LoggerWorker can accpet connections
     */
    bool _loggableGameState();
};

#endif
