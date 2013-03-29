#ifndef _PongServer
#define _PongServer

#include <QWidget>
#include <QThread>
#include <QVector>
#include <QMutex>

#include <utility>

#include "Workers.hpp"
#include "PlayingArea.hpp"
#include "PlayerState.hpp"

class PongServer
{
public:
  PongServer(const int & maxPlayers=6,
	     const int & renderAreaWidth=256,
	     const Ball & ball);

private:
  short _maxPlayers;

  //shared memories
  PlayingArea _playingArea; //shared memory
  bool _stopped; //shared memory (read only for other threads)
  Qvector<PlayerState> _playersStates; //shared memory

  //Mutexes for shared memories
  QMutex _playingAreaMutex;
  QMutex _stoppedMutex;
  QVector<QMutex> _playersStatesMutexes;

  //network
  QTcpServer _tcpServer;
  QVector<QTcpSocket*> _sockets;

  //worker classes for threads
  GameStateWorker _gameStateChecker;
  LoggerWorker _playerLogger;
  QVector<SocketWorker> _playersInterfaces;

  //thread objects
  QThread _gameStateCheckerThread;
  QThread _playerLoggerThread;
  QVector<QThread> _playersInterfacesThreads;

private slots:
  void _playerLoggedSlot(); //active le bouton "begin"
  void _beginSlot();
};

#endif