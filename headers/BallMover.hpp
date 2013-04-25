#ifndef BALLMOVER_HPP
#define BALLMOVER_HPP

#include <cmath>

#include <QString>
#include <QTimer>

#include "Concurrent.hpp"
#include "PongShared.hpp"
#include "PongTypes.hpp"

class BallMover : public Concurrent
{
    Q_OBJECT
public:
    BallMover(const qint32 & period=2);

    ~BallMover();

    void setPeriod(const qint32 & period);

    qint32 period()const;

signals:
    void appendStatusSignal(QString status);

public slots:
    void startMoving();
    void moveBall();
    void stopMoving();
    void notBusyQuit();

private:
    QTimer _timer;
    qint32 _period;
};

#endif // BALLMOVER_HPP
