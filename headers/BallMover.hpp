#ifndef BALLMOVER_HPP
#define BALLMOVER_HPP

#include <cmath>

#include <QString>
#include <QTimer>

#include "Concurrent.hpp"
#include "PongShared.hpp"
#include "PongTypes.hpp"

/*!
 * \brief The BallMover class
 * This class is intended to move the ball, and is runned by a thread, which thread is declared in the PongServer class and started in it methode start()
 */
class BallMover : public Concurrent
{
    Q_OBJECT
public:
    /*!
     * \brief constructeur
     * \param period time intervalle after wich the position of the ball is updated.
     */
    BallMover(const qint32 & period=1);

    ~BallMover();

    /*!
     * \brief set the the time intervalle between two updates of the position of the ball.
     * \param period time intervalle after wich the ball is move
     */
    void setPeriod(const qint32 & period);

    /*!
     * \brief gets the time intervalle between two updates of the position of the ball.
     * \return moving ball period, in milliseconds
     */
    qint32 period()const;

signals:
    /*!
     * \brief appends a status message in the servr view
     * This method was meant for debug purposes.
     * \param status a message to be dispalyed in server view
     */
    void appendStatusSignal(QString status);

public slots:
    /*!
     * \brief starts pediodically updating the position of the ball.
     * connected to GameStateWorker::startMovingBall() signal by PongServer, begins to update the position of the ball in PongShared::PlayingArea.
     */
    void startMoving();

    /*!
     * \brief moveBall updates the position of the ball in PongShared::PlayingArea
     * This slot is internally connected to a timer timeout() signal, to change the position of the ball periodically.
     */
    void moveBall();

    /*!
     * \brief stop updating the position of the ball
     */
    void stopMoving();

    /*!
     * \brief Allow to stop the running thread when no method or slot is currently executed.
     * In general cases, break points are present in the code of each method, sothat when a PongTypes::EXIT_REQUESTED state is encountered, the job stops and the signal finishedSignal() is emitted.
     * finishedSignal() is inherited from the class Concurrent. This signal is intended to be connected with the affinited thread's slot quit().
     */
    void notBusyQuit();

private:
    QTimer _timer; /*!< periodically calls moveBall() */
    qint32 _period; /*!< the time intervalle between two updates of the position of the ball*/
};

#endif // BALLMOVER_HPP
