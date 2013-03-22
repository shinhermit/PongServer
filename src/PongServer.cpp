#include "PongServer.hpp"

PongServer::PongServer(const int & maxPlayers,
		       const int & renderAreaWidth,
		       const Ball & ball)
  : _maxPlayers(maxPlayers),
    _playingArea(renderAreaWidth, ball),
    _gameStateChecker(_playingArea, _playersStates, _stopped),
    _playerLogger(_playingArea, _playersStates, _stopped, _playersInterfaces, _playersInterfacesThreads, _playersStates)
{
  connect(_gameStateCheckerThread, SIGNAL(started()), gameStateChecker, SLOT(checkState()));

  connect(_playersLoggerThread, SIGNAL(started()), playersLogger, SLOT(waitConnection()));

  _gameStateChecker.moveToThread(_gameStateCheckerThread);
  _playersLogger.moveToThread(_playersLoggerThread);
}

void PongServer::_begin()
{
  _gameStateCheckerThread.start();
  _playerLoggerThread.start();
}
