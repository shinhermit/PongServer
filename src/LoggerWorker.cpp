#include "LoggerWorker.hpp"

LoggerWorker::LoggerWorker(PlayingArea & playingArea,
	     const bool & stopped,
	     QVector<SocketWorker> & playersInterfaces,
	     QVector<QThread> & playersInterfacesThreads,
	     Qvector<PlayerState> & playersStates,
	     const qint16 & port)
  : _port(port),
    _playingArea(playingArea),
    _stopped(stopped),
    _playersSockets(playersInterfaces),
    _playersSocketsThreads(playersInterfacesThreads),
    _playersStates(playersStates)

{}

void LoggerWorker::waitConnection()
{
  _server.listen(QHostAddress::Any, _port);

  _socket = _server.nextPendingConnection();

  _playersInterfaces.pushback(SocketWorker(_socket, playingArea, stopped, playersStates));
  SocketWorker & interface = *_playersInterfaces.rbegin();

  _playersInterfacesThreads.push_back(QThread());
  QThread & interfaceThread = *_playersInterfacesThreads.rbegin();

  connect(interfaceThread, SIGNAL(started()), interface, SLOT(beginInteract()));
     interface.moveToThread(interfaceThread);
}

