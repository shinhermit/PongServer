#include "PongServer.hpp"

PongServer::PongServer(const int & maxPlayers,
		       const int & renderAreaWidth,
		       const Ball & ball)
  : _maxPlayers(maxPlayers),
    _playingArea(renderAreaWidth, ball),
    _gameStateChecker(_playingArea, _playersStates, _stopped),
    _playerLogger(_playersSockets, _playersSocketsThreads, _playersStates)
{
  connect(gameStateCheckerThread, SIGNAL(started()), gameStateChecker, SLOT(checkState()));

  connect(_playersLoggerThread, SIGNAL(started()), playersLogger, SLOT(waitConnection()));
}

void PongServer::_begin()
{
  _gameStateCheckerThread.start();
  _playerLoggerThread.start();
}
