#ifndef _PongTypes
#define _PongTypes

struct PongTypes
{
    enum E_PlayerState{
        CONNECTED,
        WAITING,
        ACCEPTED,
        READY,
        PLAYING,
        DISCARDED,
        DISCONNECTED,
        SOCKET_ERROR
    };

    enum E_GameState{
        NOPARTY,
        WAITING_SERVER,
        START_REQUESTED,
        INITIALIZING,
        RUNNING,
        PAUSED,
        GAMEOVER,
        EXIT_REQUESTED,
        STATE_ERROR
    };

    enum E_BallDir{
        RIGHT,
        LEFT,
        UP,
        DOWN,
        RND_DIR
    };
};

#endif
