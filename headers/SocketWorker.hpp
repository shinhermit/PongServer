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

/*!
 * \brief The SocketWorker class manages the comminucation with a client
 */
class SocketWorker : public Concurrent
{
    Q_OBJECT
public:
    /*!
     * \brief Constructor
     * \param socket the socket to which the client is connected
     * \param playerIndex the id of the connected player
     * \param parent passed to the superclass QObject
     */
    SocketWorker(QTcpSocket * socket,
                 const qint32 & playerIndex,
                 QObject * parent=0);

    ~SocketWorker();

    /*!
     * \brief for serialization
     * \param out a QDataStream
     */
    void operator>>(QDataStream & out);

    /*!
     * \brief for deserialization
     * \param in a QDataStream
     */
    void operator<<(QDataStream & in);

    /*!
     * \brief convenience functions for standard C++ stream notation
     */
    friend QDataStream & operator<<(QDataStream & out, SocketWorker & sckw);

    /*!
     * \brief convenience functions for standard C++ stream notation
     */
    friend QDataStream & operator>>(QDataStream & in, SocketWorker & sckw);

signals:
    /*!
     * \brief appends a status message to the PongServerView
     *This signal should be connected to the LoggerWorker::appendStatusSlot() of the LoggerWorker that creates the SockerWorker
     * \param status the text message to be appended
     */
    void appendStatusSignal(const QString & status);

    /*!
     * \brief Resquests to send data to the client
     *This signal should be internally connected to the sendDataSlot()
     */
    void sendDataSignal();

public slots:
    /*!
     * \brief starts interaction with the client
     *This slot should be connected to LoggerWorker::startService() or PongServer::startService() signals
     */
    void beginInteract();

    /*!
     * \brief sends useful data to the client
     *This slot should be internally connected to the sendDataSignal()
     */
    void sendDataSlot();

    /*!
     * \brief receives useful data from the client
     *This slot should be connected the internal socket's QTcpSocket::readyRead() signal
     */
    void getDataSlot();

    /*!
     * \brief manages any socket error break out
     * \param socketError error code shipped with the error signal of the socket
     *This slot should be connected to the QTcpSockert::error signal of the internal socket
     */
    void socketError( QAbstractSocket::SocketError socketError );

    /*!
     * \brief manages disconnection of the player
     *This slot should be connected to the QTcpSocket::disconnected slot of the internal socket
     */
    void disconnected();

private:
    qint32 _playerIndex; /*!< index of the dedicated player*/
    QTcpSocket * _socket; /*!< socket dedicated to the comminicatio with the client*/
    bool _disconnected; /*!< true once the player state is found equals to PongTypes::DISCONNECTED*/

    /*!
     * \brief tells if a game is currently being played
     * \return true is the state of the game is PongTypes::RUNNING, false elsewise
     */
    bool _running_state();

    /*!
     * \brief Tells if the dedicated player is in an active state
     * \return true if the state of the player is neither PongTypes::DISCONNECTED, neither PongTypes::DISCARDED, and false elsewise
     */
    bool _active_player();
};

#endif
