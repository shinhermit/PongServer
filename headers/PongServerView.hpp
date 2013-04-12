#ifndef PONGSERVERVIEW_HPP
#define PONGSERVERVIEW_HPP

#include <QWidget>

#include "Lockable.hpp"
#include "ui_PongServerView.hpp"

class PongServerView : public QWidget, public Lockable
{
    Q_OBJECT
public:
    PongServerView();
    void setStatus(const QString & status);
    void appendStatus(const QString & status);
    void enableStartButton();
    void disableStartButton();

    QString status()const;

signals:
    void startClickedSignal();
    void closeSignal();

private slots:
    void startClickedSlot();
    void quitClickedSlot();

private:
    Ui_PonServerView _ui;
};

#endif // PONGSERVERVIEW_HPP
