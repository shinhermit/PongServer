#include "PlayersStatesHolder.hpp"

PlayersStatesHolder::PlayersStatesHolder():
    _upToDate(false)
{}

void PlayersStatesHolder::add(const PlayerState &playerState)
{
    _playersStates.push_back(playerState);
}

void PlayersStatesHolder::erase(const qint32 &pos)
{
    _playersStates.erase(_playersStates.begin()+pos);
}

void PlayersStatesHolder::erase(const PlayersStatesHolder::Iterator &it)
{
    _playersStates.erase(it);
}

void PlayersStatesHolder::remove(const PlayerState &playerState)
{
    _playersStates.remove( _playersStates.indexOf(playerState) );
}

const PlayerState & PlayersStatesHolder::at(const qint32 &pos)
{
    return _playersStates[pos];
}

PlayerState &PlayersStatesHolder::operator [](const qint32 &pos)
{
    return _playersStates[pos];
}

qint32 PlayersStatesHolder::indexOf(const PlayerState &playerState)
{
    return _playersStates.indexOf(playerState);
}

qint32 PlayersStatesHolder::size() const
{
    return _playersStates.size();
}

PlayersStatesHolder::Iterator PlayersStatesHolder::begin()
{
    return _playersStates.begin();
}

PlayersStatesHolder::Iterator PlayersStatesHolder::end()
{
    return _playersStates.end();
}

void PlayersStatesHolder::askShareState()
{
    emit shareStateDemand();
}

void PlayersStatesHolder::askShareState(const qint32 &playerIndex)
{
    emit shareStateDemand(playerIndex);
}

bool PlayersStatesHolder::upToDate() const
{
    return _upToDate;
}

void PlayersStatesHolder::setState(QVector<PlayerState> playersStates)
{
    _playersStates = playersStates;
}

void PlayersStatesHolder::setState(PlayerState playerState, qint32 playerIndex)
{
    _playersStates[playerIndex] = playerState;
}

void PlayersStatesHolder::shareState()
{
    emit setStateDemand(_playersStates);
}

void PlayersStatesHolder::shareState(qint32 playerIndex)
{
    emit setStateDemand(_playersStates[playerIndex], playerIndex);
}
