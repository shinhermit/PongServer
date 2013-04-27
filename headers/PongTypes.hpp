#ifndef _PongTypes
#define _PongTypes

/*!
 * \brief The PongTypes struct holds some usful constants
 */
struct PongTypes
{
    /*!
     * \brief The E_PlayerState enum represents possible the states of a player
     *Much values have been anticipated, but not used afterward
     */
    enum E_PlayerState{
        CONNECTED, /*!< the player is connected*/
        WAITING, /*!< the player is wating to be inserted into a party*/
        ACCEPTED, /*!< the player has been accepted*/
        READY, /*!< the player is ready to play*/
        PLAYING, /*!< the player is playing a party*/
        DISCARDED, /*!< the player has been discarded*/
        DISCONNECTED, /*!< the player is disconnected*/
        SOCKET_ERROR /*!< an error occured with the socket*/
    };

    /*!
     * \brief The E_GameState enum represents the possible states of the game
     *Some values have been anticipated, but not used afterward
     */
    enum E_GameState{
        NOPARTY, /*!< Intial state*/
        WAITING_SERVER, /*!< Worker must wait for the start order*/
        START_REQUESTED, /*!< A start order has been sent */
        INITIALIZING, /*!< The game is being initialized. Next step is playing*/
        RUNNING, /*!< A party is in play*/
        PAUSED, /*!< The game has been paused*/
        GAMEOVER, /*!< A gameover state has been detected*/
        RESET_REQUESTED, /*!< A new game is requested*/
        EXIT_REQUESTED, /*!< Quit game requested. All thread must terminated.*/
        STATE_ERROR /*!< Any error*/
    };

    /*!
     * \brief The E_BallDir enum was anticipated, but not used as of now
     */
    enum E_BallDir{
        RIGHT, /*!< Set ball direction to 0°*/
        LEFT, /*!< Set ball direction to 180°*/
        UP, /*!< Set ball direction to 90°*/
        DOWN, /*!< Set ball direction to 270°*/
        RND_DIR /*!< Set ball direction randomly*/
    };
};

#endif
