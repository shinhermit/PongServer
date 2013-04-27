#ifndef LOBBYAGENT_HPP
#define LOBBYAGENT_HPP

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QDebug>
#include <QDataStream>
#include <QByteArray>
#include <QTimer>
#include <QErrorMessage>

#include "PongShared.hpp"
#include "Concurrent.hpp"

/*!
 * \brief The LobbyAgent class is intended to manage the communication with the Lobby.
 *The role of the Lobby, writtent in Java, is to route the clients to the server. The server receives from the Lobby the number of clients to wait for, and starts the game after a corresponding number of players are connected.
 *This class accesses PongShared objects, thus inherits the class Concurrent.
 */
class LobbyAgent : public Concurrent
{
    Q_OBJECT
public:
    /*!
     * \brief Constructor
     * \param socket the socket to use for tcp connection to the Lobby
     */
    LobbyAgent(QTcpSocket * socket);

    ~LobbyAgent();

    /*!
     * \brief establishes the connection to the Lobby
     * \param host ipv4 address of hostname
     * \param port port where the Lobby listens
     */
    void connectToLobby(const QString & host="127.0.0.1", const qint16 & port=6666);

    /*!
     * \brief receives data from the Lobby through the socket through the QDataStream
     * \param in a stream associated to the socket
     */
    void operator<<(QDataStream & in);

signals:
    /*!
     * \brief signal emitted when the number of connected players equals to the number to wait specified by the Lobby.
     *This signal should be connected inside PongSer to the slot PongServer::startRequested()
     */
    void startSignal();

    /*!
     * \brief appens a status message in the PongServer view
     *This signal should be connected inside PongServer to the appendStatusSlot of PongServerView
     * \param status the message to be appended
     */
    void appendStatus(QString status);

    /*!
     * \brief start the routine that periodically checks the number of connected players.
     *emitted after the number of players to wait for is received from the Lobby. This signal should be internally connected to the slot checkNbPlayers()
     */
    void beginCheckNbPlayers();

public slots:
    /*!
     * \brief gets the number of players to wait for from the Lobby
     *This slot should be internally connected to the readyRead() signal of the socket
     */
    void ordersReceived();

    /*!
     * \brief start the routine that periodically checks the number of connected players
     */
    void checkNbPlayers();

private slots:
    /*!
     * \brief treats socket errors
     * \param socketError error code received via the error signal emitted by the socket
     */
    void _socketError(QAbstractSocket::SocketError socketError);

    /*!
     * \brief the actual routine that periodically checks the number of connected players in PongShared::gameState
     */
    void _check_NbPlayers_routine();

    /*!
     * \brief gets the number of connected players from PongShared::gameState
     * \return the number of players currently set as connected
     */
    qint32 _nbPlayers();

private:
    QTcpSocket * _socket; /*!< the socket for comminicating with the Lobby*/
    qint32 _nbPlayersTowait; /*!< the number of players to wait before starting the game*/
    bool _startSignalSent; /*!< flag used to avoid sending several times the startSignal*/
};

#endif // LOBBYAGENT_HPP
