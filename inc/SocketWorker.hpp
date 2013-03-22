#ifndef _SocketWorker
#define _SocketWorker

#include <QObject>
#include <QTcpSocket>

class SocketWorker : public QObject
{
Q_OBJECT

public:
  SocketWorker(QTcpSocket & _socket,
	       PlayingArea & playingArea,
	       const bool & stopped,
	       Qvector<PlayerState> & playersStates);

public slots:
  void beginInteract();
};

#endif
