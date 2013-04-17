#include "PlayerState.hpp"

const qint32 PlayerState::_defaultCredit = 3;

PlayerState::PlayerState(
        const qint32 & id,
        const PongTypes::E_PlayerState & state,
        QObject * parent
        ):
    QObject(parent),
    _myIndex(id),
    _dxRacket(3),
    _credit(_defaultCredit),
    _state(state)
{}

PlayerState::PlayerState(const PlayerState &source):
    QObject( source.parent() ),
    _myIndex(source._myIndex),
    _dxRacket(source._dxRacket),
    _credit(source._credit),
    _state(source._state)
{}

PlayerState &PlayerState::operator =(const PlayerState &source)
{
    _myIndex = source._myIndex;
    _dxRacket = source._dxRacket;
    _credit = source._credit,
    _state = source._state;

    return *this;
}

bool PlayerState::operator ==(const PlayerState &ref) const
{
    return (_myIndex == ref._myIndex
            &&
            _dxRacket == ref._dxRacket
            &&
            _credit == ref._credit
            &&
            _state == ref._state);
}

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
