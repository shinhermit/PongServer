#include "LoggerWorker.hpp"

LoggerWorker::LoggerWorker(QTcpServer & tcpServer,
			   QVector<QTcpSocket*> & sockets,
			   PlayingArea & playingArea,
			   const bool & stopped,
			   QVector<SocketWorker> & playersInterfaces,
			   QVector<QThread> & playersInterfacesThreads,
			   Qvector<PlayerState> & playersStates,
			   const qint16 & port)
  : _tcpServer(tcpServer),
    _sockets(sockets),
    _port(port),
    _playingArea(playingArea),
    _stopped(stopped),
    _playersSockets(playersInterfaces),
    _playersSocketsThreads(playersInterfacesThreads),
    _playersStates(playersStates)

{}

void LoggerWorker::waitConnection()
{
  _server.listen(QHostAddress::Any, _port);

  connect(_server, SIGNAL(newConnection()), this, SLOT(newConnectionSlot()));
}

void LoggerWorker::newConnectionSlot()
{
  _sockets.push_back( _server.nextPendingConnection() );
  QSocket & socket = **_sockets.rbegin();

  _playersInterfaces.pushback(SocketWorker(socket, playingArea, stopped, playersStates));
  SocketWorker & interface = *_playersInterfaces.rbegin();

  _playersInterfacesThreads.push_back(QThread());
  QThread & interfaceThread = *_playersInterfacesThreads.rbegin();

  connect(interfaceThread, SIGNAL(started()), interface, SLOT(beginInteract()));
     interface.moveToThread(interfaceThread);
}
