#ifndef PONGSERVERVIEW_HPP
#define PONGSERVERVIEW_HPP

#include <QWidget>

namespace Ui {
    class PongServerView;
}

class PongServerView : public QWidget
{
    Q_OBJECT
public:
    PongServerView();
    ~PongServerView();

    void setStatus(const QString & status);
    void appendStatus(const QString &status);
    void enableStartButton();
    void disableStartButton();

    void disableQuitButton();

    QString status()const;
    QString lobbyHost()const;
    qint16 lobbyPort()const;

signals:
    void startClickedSignal();
    void exitSignal();

public slots:
    void appendStatusSlot(const QString & status);

private slots:
    void startClickedSlot();
    void quitClickedSlot();

private:
    Ui::PongServerView * _ui;
};

#endif // PONGSERVERVIEW_HPP
