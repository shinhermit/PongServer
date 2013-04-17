#ifndef BALLMOVER_HPP
#define BALLMOVER_HPP

#include <cmath>

#include <QString>
#include <QTimer>

#include "Concurrent.hpp"
#include "PongShared.hpp"
#include "PongTypes.hpp"

class BallMover : public QObject, public Concurrent
{
    Q_OBJECT
public:
    BallMover(const qint32 & period=10);

    ~BallMover();

    void setPeriod(const qint32 & period);

    qint32 period()const;

signals:
    void finishedSignal();
    void appendStatusSignal(QString status);

public slots:
    void startMoving();
    void moveBall();
    void stopMoving();

private:
    QTimer _timer;
    qint32 _period;

    bool _exit_requested();
};

#endif // BALLMOVER_HPP
