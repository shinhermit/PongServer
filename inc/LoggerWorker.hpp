#ifndef _LoggerWorker
#define _LoggerWorker

#include <QObject>
#include <QThread>
#include <QVector>

#include "PlayerState.hpp"
#include "SocketWorker.hpp"

class LoggerWorker : public QObject
{
Q_OBJECT

public:
  LoggerWorker(QVector<SocketWorker> & _playersSockets,
	       QVector<QThread> & _playersSocketsThreads,
	       Qvector<PlayerState> & _playersStates);

signals:
  void _playerLoggedSignal(); //active le bouton "begin"

public slots:
  void waitConnection();
}

#endif
