#ifndef _PlayerState
#define _PlayerState

#include <cmath>

#include <QObject>
#include <QLineF>

#include "PongTypes.hpp"
#include "Lockable.hpp"

class PlayerState : public QObject, public Lockable
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

    bool operator==(const PlayerState & ref)const;

    void setId(const qint32 & id);
    void setCredit(const qint32 &credit);
    void decreaseCredit(const qint32 & step=1);
    void setState(const PongTypes::E_PlayerState & state);
    void setFailState();
    void setRacket(const QLineF & racket);
    void setRacket(const QPointF & p1, const QPointF & p2);
    void setRacket(const qreal & x1,
                   const qreal & y1,
                   const qreal & x2,
                   const qreal & y2);

    const qint32 & id()const;
    const QLineF & racket()const;
    const qint32 & credit()const;
    const PongTypes::E_PlayerState & state()const;

    static const qint32 &DefaultCredit();

private:
    qint32 _myIndex;
    QLineF _racket;
    qint32 _credit;
    PongTypes::E_PlayerState _state;

    static const qint32 _defaultCredit;
};

#endif
