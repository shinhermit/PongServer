#ifndef _PongTypes
#define _PongTypes

struct PongTypes
{
  enum E_PlayerState{WAITING, PLAYING, DISCARDED};

  enum E_GameState{GAMEOVER, PAUSED, EXITED, RUNNING, NOPARTY};
};

#endif
