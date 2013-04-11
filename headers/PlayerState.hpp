#ifndef _PlayerState
#define _PlayerState

#include <QObject>
#include <QLineF>

#include "PongTypes.hpp"
#include "Lockable.hpp"

class PlayerState : public QObject, public Lockable
{
    Q_OBJECT
public:
    PlayerState(QObject & parent,
                const qreal & dxRacket,
                const qint32 &credit,
                const PongTypes::E_PlayerState & state=PongTypes::WAITING);

    PlayerState(const qreal & dxRacket,
                const qint32 & credit,
                const PongTypes::E_PlayerState & state=PongTypes::WAITING);

    PlayerState(QObject & parent,
                const qint32 & credit,
                const PongTypes::E_PlayerState & state=PongTypes::WAITING);

    PlayerState(const int & credit,
                const PongTypes::E_PlayerState & state=PongTypes::WAITING);

    PlayerState(QObject & parent,
                const PongTypes::E_PlayerState & state=PongTypes::WAITING);

    PlayerState(const PongTypes::E_PlayerState & state=PongTypes::WAITING);

    void setCredit(const qint32 &credit);
    void decreaseCredit(const qint32 & step=1);
    void setState(const PongTypes::E_PlayerState & state);
    void setFailState();
    void setdxRacket(const qreal & dxRacket);

    const qreal & dxRacket()const;
    const qint32 & credit()const;
    const PongTypes::E_PlayerState & state()const;

    static const qint32 &DefaultCredit();

private:
    qreal _dxRacket;
    qint32 _credit;
    PongTypes::E_PlayerState _state;

    static const qint32 _defaultCredit;
};

#endif
