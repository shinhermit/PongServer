#ifndef _LoggerWorker
#define _LoggerWorker

#include <QObject>
#include <QThread>
#include <QVector>
#include <QTcpServer>
#include <QTcpSocket>

#include "PlayerState.hpp"
#include "SocketWorker.hpp"

class LoggerWorker : public QObject
{
  Q_OBJECT

  public:
  LoggerWorker(PlayingArea & playingArea,
	       Qvector<PlayerState> & playersStates,
	       const bool & stopped,
	       QVector<SocketWorker> & playersInterfaces,
	       QVector<QThread> & playersInterfacesThreads,
	       Qvector<PlayerState> & playersStates,
	       const qint16 & port=7777);

signals:
  void _playerLoggedSignal(); //active le bouton "begin"

public slots:
  void waitConnection();

private:
  QTcpServer _server;
  QTcpSocket _socket;
  qint16 _port;

  PlayingArea & _playingArea;
  const bool & _stopped;
  QVector<SocketWorker> & _playersInterfaces;
  QVector<QThread> & _playersInterfacesThreads;
  Qvector<PlayerState> & _playersStates;
};

#endif
