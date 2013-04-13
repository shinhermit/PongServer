#include "PlayerState.hpp"

const qint32 PlayerState::_defaultCredit = 3;

PlayerState::PlayerState(
        const qint32 & id,
        const PongTypes::E_PlayerState & state,
        QObject * parent
        ):
    QObject(parent),
    _myIndex(id),
    _dxRacket(0),
    _credit(_defaultCredit),
    _state(state)
{}

void PlayerState::setId(const qint32 & id)
{
    _myIndex = id;
}

void PlayerState::setCredit(const qint32 &credit)
{
    _credit = ::abs(credit);
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

const qint32 &PlayerState::id() const
{
    return _myIndex;
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
