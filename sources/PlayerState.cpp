#include "PlayerState.hpp"

const qint32 PlayerState::_defaultCredit = 3;


PlayerState::PlayerState(QObject & parent,
                         const qreal &dxRacket,
                         const qint32 &credit,
                         const PongTypes::E_PlayerState &state):
    QObject(&parent),
    _dxRacket(dxRacket),
    _credit(credit),
    _state(state)
{}

PlayerState::PlayerState(const qreal &dxRacket,
                         const qint32 &credit,
                         const PongTypes::E_PlayerState &state):
    _dxRacket(dxRacket),
    _credit(credit),
    _state(state)
{}

PlayerState::PlayerState(QObject &parent,
                         const qint32 & credit,
                         const PongTypes::E_PlayerState & state):
    QObject(&parent),
    _dxRacket(0),
    _credit(credit),
    _state(state)
{}

PlayerState::PlayerState(const int & credit,
                         const PongTypes::E_PlayerState & state):
    _dxRacket(0),
    _credit(credit),
    _state(state)
{}

PlayerState::PlayerState(QObject &parent,
                         const PongTypes::E_PlayerState & state):
    QObject(&parent),
    _dxRacket(0),
    _credit(_defaultCredit),
    _state(state)
{}

PlayerState::PlayerState(const PongTypes::E_PlayerState & state):
    _dxRacket(0),
    _credit(_defaultCredit),
    _state(state)
{}

void PlayerState::setCredit(const qint32 &credit)
{
    _credit = credit;
}

void PlayerState::decreaseCredit(const qint32 &step)
{
    _credit -= step;
    if(_credit < 0)
        _credit = 0;
}

void PlayerState::setState(const PongTypes::E_PlayerState &state)
{
    _state = state;
}

void PlayerState::setFailState()
{
    _state = PongTypes::DISCARDED;
}

void PlayerState::setdxRacket(const qreal & dxRacket)
{
    _dxRacket = dxRacket;
}

const qreal &PlayerState::dxRacket() const
{
    return _dxRacket;
}

const qint32 &PlayerState::credit() const
{
    return _credit;
}

const PongTypes::E_PlayerState &PlayerState::state() const
{
    return _state;
}

const qint32 &PlayerState::DefaultCredit()
{
    return _defaultCredit;
}
