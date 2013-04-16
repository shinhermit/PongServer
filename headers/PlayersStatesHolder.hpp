#ifndef PLAYERSSTATESHOLER_HPP
#define PLAYERSSTATESHOLER_HPP

#include <QObject>
#include <QVector>

#include "PlayerState.hpp"

class PlayersStatesHolder : public QObject
{
    Q_OBJECT
public:
    typedef QVector<PlayerState>::iterator Iterator;
    PlayersStatesHolder();

    void add(const PlayerState & playerState);

    void erase(const qint32 & pos);
    void erase(const Iterator & it);

    void remove(const PlayerState & playerState);

    const PlayerState & at(const qint32 & pos);
    PlayerState & operator[](const qint32 & pos);
    qint32 indexOf(const PlayerState & playerState);
    qint32 size()const;

    Iterator begin();
    Iterator end();


//synchronization

    void askShareState();
    void askShareState(const qint32 & playerIndex);

    bool upToDate()const;

public slots:
    void setState(QVector<PlayerState> playersStates);
    void setState(PlayerState playerState, qint32 playerIndex);

    void shareState();
    void shareState(qint32 playerIndex);

signals:
    void setStateDemand(QVector<PlayerState> playersStates);
    void setStateDemand(PlayerState playerState, qint32 playerIndex);

    void shareStateDemand();
    void shareStateDemand(qint32 playerIndex);

private:
    QVector<PlayerState> _playersStates;
    bool _upToDate;
};

#endif // PLAYERSSTATESHOLER_HPP
