#ifndef _PongServer
#define _PongServer

#include <QWidget>
#include <QThread>
#include <QVector>

#include <utility>

#include "Workers.hpp"
#include "PlayingArea.hpp"
#include "PlayerState.hpp"

class PongServer : public QWidget
{
Q_OBJECT

public:
  PongServer(const int & maxPlayers=6,
	     const int & renderAreaWidth=256,
	     const Ball & ball);

private:
  short _maxPlayers;
  PlayingArea _playingArea; //shared memory
  bool _stopped; //shared memory (read only for other threads)

  QTcpServer _tcpServer;
  QVector<QTcpSocket*> _sockets;

  GameStateWorker _gameStateChecker;
  LoggerWorker _playerLogger;
  QVector<SocketWorker> _playersInterfaces;
  Qvector<PlayerState> _playersStates; //shared memory

  QThread _gameStateCheckerThread;
  QThread _playerLoggerThread;
  QVector<QThread> _playersInterfacesThreads;

private slots:
  void _playerLoggedSlot(); //active le bouton "begin"
  void _beginSlot();
};

#endif
