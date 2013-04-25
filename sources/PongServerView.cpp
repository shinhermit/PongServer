#include "PongServerView.hpp"
#include "ui_PongServerView.h"

PongServerView::PongServerView()
{
    _ui = new Ui::PongServerView();
    _ui->setupUi(this);
    _ui->statusText->setPlainText(". PongServer Ui initialized\n");
}

PongServerView::~PongServerView()
{
    delete _ui;
}

void PongServerView::setStatus(const QString &status)
{
    _ui->statusText->setPlainText(". " + status + "\n");
}

void PongServerView::appendStatus(const QString & status)
{
    _ui->statusText->setPlainText( ". " + status + "\n" + _ui->statusText->toPlainText() );
}

void PongServerView::enableStartButton()
{
    _ui->startButton->setEnabled(true);
}

void PongServerView::disableStartButton()
{
    _ui->startButton->setEnabled(false);
}

void PongServerView::disableQuitButton()
{
    _ui->quitButton->setEnabled(false);
}


QString PongServerView::status() const
{
    return _ui->statusText->toPlainText();
}

QString PongServerView::lobbyHost() const
{
    return _ui->lobbyHost->text();
}

qint16 PongServerView::lobbyPort() const
{
    return (qint16)_ui->lobbyPort->value();
}


void PongServerView::startClickedSlot()
{
    emit startClickedSignal();
}

void PongServerView::quitClickedSlot()
{
    emit exitSignal();
}


void PongServerView::appendStatusSlot(const QString &status)
{
    appendStatus(status);
}
