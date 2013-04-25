#ifndef CONCURRENT_HPP
#define CONCURRENT_HPP

#include <QString>
#include <QDebug>

#include "PongShared.hpp"

class Concurrent : public QObject
{
    Q_OBJECT
public:
    Concurrent(QObject * parent=0);

    void lockGameState();
    void lockPlayingArea();
    void lockPlayersStates();

    void unlockGameState();
    void unlockPlayingArea();
    void unlockPlayersStates();

public slots:
    void notBusyQuit();

protected:
    bool _exit_requested();

signals:
    void finishedSignal();

private:
    bool _I_locked_gameState;
    bool _I_locked_playingArea;
    bool _I_locked_playersStates;
    QString _lockerName;//debug
};

#endif // CONCURRENT_HPP
