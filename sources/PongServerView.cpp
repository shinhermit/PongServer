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
    _ui->statusText->setPlainText(". " + status);
}

void PongServerView::appendStatus(const QString & status)
{
    _ui->statusText->setPlainText( _ui->statusText->toPlainText() + "\n. " + status);
}

void PongServerView::enableStartButton()
{
    _ui->startButton->setEnabled(true);
}

void PongServerView::disableStartButton()
{
    _ui->startButton->setEnabled(false);
}


QString PongServerView::status() const
{
    return _ui->statusText->toPlainText();
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
