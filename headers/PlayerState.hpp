#ifndef _PlayerState
#define _PlayerState

#include <cmath>

#include <QObject>
#include <QLineF>

#include "PongTypes.hpp"

class PlayerState : public QObject
{
    Q_OBJECT
public:
    PlayerState(
            const qint32 & id=0,
            const PongTypes::E_PlayerState & state=PongTypes::WAITING,
            QObject * parent=0
            );

    PlayerState(const PlayerState & source);

    PlayerState & operator=(const PlayerState & source);

    bool operator==(const PlayerState & source)const;

    void setId(const qint32 & id);
    void setCredit(const qint32 &credit);
    void decreaseCredit(const qint32 & step=1);
    void setState(const PongTypes::E_PlayerState & state);
    void setFailState();
    void setdxRacket(const qreal & dxRacket);

    const qint32 & id()const;
    const qreal & dxRacket()const;
    const qint32 & credit()const;
    const PongTypes::E_PlayerState & state()const;

    static const qint32 &DefaultCredit();

private:
    qint32 _myIndex;
    qreal _dxRacket;
    qint32 _credit;
    PongTypes::E_PlayerState _state;

    static const qint32 _defaultCredit;
};

#endif
