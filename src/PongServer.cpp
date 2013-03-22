#include "PongServer.hpp"

PongServer::PongServer(const int & maxPlayers=6,
		       const int & renderAreaWidth=256,
		       const std::pair<int,int> & ballPosition)
  : _maxPlayers(maxPlayers),
    _playingArea(renderAreaWidth, ballPosition),
    _gameStateChecker(_playingArea, _playersStates, _stopped),
    _playersLogger(_playersSockets, _playersSocketsThreads, _playersStates)
{
  connect(gameStateCheckerThread, SIGNAL(started()), gameStateChecker, SLOT(checkState()));

  connect(_playersLoggerThread, SIGNAL(started()), playersLogger, SLOT(checkState()));
}

void PongServer::_begin()
{
  _gameStateCheckerThread.start();
  _playerLoggerThread.start();
}
