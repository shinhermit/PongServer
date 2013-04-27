#ifndef _PongServer
#define _PongServer

#include <QWidget>
#include <QThread>
#include <QVector>
#include <QMutex>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "Workers.hpp"
#include "Concurrent.hpp"
#include "PongShared.hpp"
#include "PongServerView.hpp"

/*!
 * \brief The PongServer class is the game engine
 */
class PongServer : public Concurrent
{
    Q_OBJECT
public:
    /*!
     * \brief Constructor
     * \param port the port on which client will connect
     */
    PongServer(const qint16 & port=6666);

    /*!
     * \brief Starts the engine
     */
    void start();

    /*!
     * \brief Displays the internal scene.
     *Use for debug only
     */
    void showScene();

signals:
    /*!
     * \brief Signals and error when using PongTypes::E_gameState
     *This signal was anticipated but not used as of now
     * \param mess message to be printed
     */
    void gameStateErrorSignal(const QString & mess);

    /*!
     * \brief Signals the need for a new game
     *This signal should be internally connected to the slot PongServer::newGameSlot()
     */
    void newGameSignal();

    /*!
     * \brief Starts the worker objects
     *This signal should be connected to the slots of worker classes that begin their work. Worker classes include:
     *. BallMover
     *. GameStateWorker
     *. LobbyAgent
     *. LoggerWorker
     */
    void startService();

    /*!
     * \brief Stops the worker objects
     *Dual of PongServer::startService()
     */
    void stopService();


public slots:
    /*!
     * \brief Attempts to connect to the Lobby
     *This Slot should be connected to the signal PongServerView::connectToLobbySignal(), which itself responds to the signal clicked of the button connect on the PongServerView GUI.
     */
    void connectToLobby();

    /*!
     * \brief Initializes a new game
     *This slot should be connected to:
     *. PongServer::newGameSignal
     *. GameStateWorker::gameOverSignal
     */
    void newGameSlot();

    /*!
     * \brief Sets the state of the game to PongTypes::STATE_ERROR
     *This slot should ne connected to PongServer::gameStateErrorSignal
     * \param mess message to be printed
     */
    void gameStateErrorSlot(const QString & mess);

    /*!
     * \brief Performs a number of actions when a new players is connected.
     *It desables the start button if not enough player remains connected
     *This slot should be connected to LoggerWorker::newPlayerConnected() signal
     */
    void newPlayerConnected();

    /*!
     * \brief Sets the state of the game to PongTypes::START_REQUESTED
     *This slot should be connected to PongServerView::startClickedSignal()
     */
    void startRequestedSlot();

    /*!
     * \brief wait a moment and then emits newGameSignal
     *This slot should be connected to GameStateWorker::gameOverSignal
     */
    void gameOverSlot();

    /*!
     * \brief Sets the state of the game to PongTypes::EXIT_REQUESTED
     *This slot should be connected to PongServerView::exitSignal()
     */
    void quitSlot();

    /*!
     * \brief Appends a status message when a threa finishes
     *This slot should be connected to the terminated() slot of any created thread in the app
     */
    void threadTerminated();

//shared stuffs

private:
    PongServerView _view; /*!< Game engine GUI*/

    QGraphicsScene _graphicScene; /*!< Used to display the scene when debugging*/

    QGraphicsView _graphicView; /*!< Used to display the scene when debugging*/

    //worker classes for threads
    GameStateWorker _gameStateChecker; /*!< Worker that perdiodically checks the state of the game*/

    LoggerWorker _playerLogger; /*!< Worker that manage new players connections*/

    BallMover _ballMover; /*!< Worker that periodically updates the position of the ball*/

    LobbyAgent _lobbyAgent; /*!< Worker that locally represents the Lobby*/

    //thread object
    QThread _gameStateCheckerThread; /*!< The thread that runs _gameStateChecker*/

    QThread _ballMoverThread; /*!< The thread that runs _ballMover */

    QThread _lobbyAgentThread; /*!< The thread that runs _lobbyAgent */

    /*!
     * \brief Resets the state of the game
     */
    void _reset_gameState();

    /*!
     * \brief resets the states of the players
     */
    void _reset_playersStates();

    /*!
     * \brief tells if all thread created in the app have finish
     * \return true if all thread finished, false elsewise
     */
    bool _all_threads_finished();
};

#endif
