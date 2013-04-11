#ifndef PONGSERVERVIEW_HPP
#define PONGSERVERVIEW_HPP

#include <QWidget>

#include "ui_PongServerView.hpp"

class PongServerView : public QWidget
{
    Q_OBJECT
public:
    PongServerView();
    void setStatus(const QString & status);
    void enableStartButton();
    void disableStartButton();

    QString status()const;

signals:
    void startClickedSignal();

private slots:
    void startClickedSlot();

private:
    Ui_PonServerView _ui;
};

#endif // PONGSERVERVIEW_HPP
