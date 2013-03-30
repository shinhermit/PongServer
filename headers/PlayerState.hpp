#ifndef _PlayerState
#define _PlayerState

#include <QObject>
#include <QLineF>

#include "PongTypes.hpp"

class PlayerState
{
public:
    PlayerState(const qreal & dxRacket,
                const int & credit,
                const PongTypes::E_PlayerState & state=PongTypes::WAITING);

    PlayerState(const int & credit,
                const PongTypes::E_PlayerState & state=PongTypes::WAITING);

    PlayerState(const PongTypes::E_PlayerState & state=PongTypes::WAITING);

    void setCredit(const int &credit);
    void decreaseCredit(const int & step=1);
    void setState(const PongTypes::E_PlayerState & state);
    void setFailState();

    const qreal & dxRacket()const;
    const int & credit()const;
    const PongTypes::E_PlayerState & state()const;

private:
    qreal _dxRacket;
    int _credit;
    PongTypes::E_PlayerState _state;

    static const int _defaultCredit;
};

#endif
