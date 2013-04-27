#ifndef _GameState
#define _GameState

#include <cmath>

#include <QDebug>

#include "PongTypes.hpp"
#include "Lockable.hpp"

/*!
 * \brief The GameState class holds the global state of the game
 *This class is inteded to be shared amoung all worker classes through the PongShared class.
 */
class GameState : public Lockable
{
public:
    /*!
     * \brief default constructor
     * \param initialState the desired initial state of the game.
     */
    GameState(const PongTypes::E_GameState & initialState=PongTypes::NOPARTY);

    /*!
     * \brief Copy constructor
     */
    GameState(const GameState & source);

    /*!
     * \brief operator =
     * \param source another GameState object
     * \return this GameState object
     */
    GameState & operator=(const GameState & source);

    /*!
     * \brief Sets the state of the game to PongTypes::GAMEOVER afted a goal.
     * \param cageIndex index of the cage where the goal appended.
     */
    void setGameOver(const qint32 & cageIndex);

    /*!
     * \brief Sets the state of the game to PongTypes::PAUSED
     *This method was anticipated, but not used as of now
     */
    void setPaused();

    /*!
     * \brief Sets the state of the game to PongTypes::EXIT_REQUESTED
     */
    void setExitRequested();

    /*!
     * \brief Sets the state of the game to PongTypes::WAITING_SERVER
     */
    void setWaitingServer();

    /*!
     * \brief Sets the state of the game to PongTypes::INITIALIZING
     */
    void setInitializing();

    /*!
     * \brief Sets the state of the game to PongTypes::RUNNING
     */
    void setRunning();

    /*!
     * \brief Sets the state of the game to PongTypes::NOPARTY
     */
    void setNoParty();

    /*!
     * \brief Sets the state of the game to PongTypes::STATE_ERROR
     */
    void setStateError();

    /*!
     * \brief Sets the state of the game to PongTypes::START_REQUESTED
     */
    void setStartRequested();

    /*!
     * \brief Sets the state of the game to PongTypes::RESET_REQUESTED
     */
    void setResetRequested();

    /*!
     * \brief Sets the index of the loser
     *The loserIndex is the index of the player that conceded the last goal, just before a gameover.
     *This method was anticipated, but not used as of now.
     * \param index the actual index.
     */
    void setLoserIndex(const qint32 & index);

    /*!
     * \brief Sets the value of the count-down before the start of the game.
     * \param value the initial value to count down from.
     */
    void setDownCounter(const qint32 & value);

    /*!
     * \brief Sets the numbers of players currently connected to the server.
     *incNbPlayers() is more used.
     * \param nbPlayers the number of players currently connected
     */
    void setNbPlayers(const qint32 & nbPlayers);

    /*!
     * \brief Sets the number of active players.
     *Active players are players who are neither in DISCARDED state, neither in DISCONNECTED stated.
     * \param nbActive the number of active players.
     */
    void setNbActive(const qint32 & nbActive);

    /*!
     * \brief Increments the number of connected players
     */
    void incNbPlayers();

    /*!
     * \brief Decrements the number of connected players
     */
    void decNbPlayers();

    /*!
     * \brief increments the number of active players
     *Active players are players who are neither in DISCARDED state, neither in DISCONNECTED stated.
     */
    void incNbActive();

    /*!
     * \brief decrements the number of active players
     *Active players are players who are neither in DISCARDED state, neither in DISCONNECTED stated.
     */
    void decNbActive();

    /*!
     * \brief Gets the current state of the game
     * \return PongTypes::E_GameState state of the game
     */
    const PongTypes::E_GameState & state()const;

    /*!
     * \brief Gets the currents value of the loserIndex
     *The loserIndex is the index of player that conceded the last goal, just before a gameover.
     * \return index of the player that loses.
     */
    const qint32 & loserIndex()const;

    /*!
     * \brief Gets the current value of the downCounter
     *At the begennning of the game, the downCounter counts down from the the value returned by this getter up to zero, then the game begeins.
     * \return value from which the count down is lunched at the beginning of the game.
     */
    const qint32 & downCounter()const;

    /*!
     * \brief Gets the numbers players currently connected to the server
     * \return number of players currently connected
     */
    const qint32  & nbPlayers()const;

    /*!
     * \brief Gets the number of active players
     *Active players are player who are neither in DISCARDED state, neither in DISCONNECTED state.
     * \return number of active players.
     */
    const qint32 & nbActive()const;

private:
    PongTypes::E_GameState _state; /*!< Current state of the game*/
    qint32 _loserIndex; /*!< current value of the index of the loser players */
    qint32 _downCounter; /*!< current value of the initial value of the beginning count down*/
    qint32 _nbPlayers; /*!< current number of connected player */
    qint32 _nbActive; /*!< current number of active players*/
};

#endif
