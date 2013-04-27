/*!
 *\file workers.hpp
 *\brief Header for all workers of the engine.
 *Workers are classes that are meant to be runned by threads, and share data through object of PongShared. The worker classes include:
 *. BallMover
 *. GameStateWorker
 *. LobbyAgent
 *. LoggerWorker
 *. SocketWorker
 *However, SocketWorker is not included in this header, because it is used only be LoggerWorker.
 */

#ifndef _Workers
#define _Workers

#include "GameStateWorker.hpp"
#include "LoggerWorker.hpp"
#include "BallMover.hpp"
#include "LobbyAgent.hpp"

#endif
