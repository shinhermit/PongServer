#include "PongServerView.hpp"

PongServerView::PongServerView()
{
    _ui.setupUi(this);
    _ui.statusText->setPlainText(". Ponserver Ui initialized\n");
}

void PongServerView::setStatus(const QString &status)
{
    _ui.statusText->setPlainText(". " + status);
}

void PongServerView::appendStatus(const QString &status)
{
    _ui.statusText->setPlainText( _ui.statusText->toPlainText() + "\n. " + status);
}

void PongServerView::enableStartButton()
{
    _ui.startButton->setEnabled(true);
}

void PongServerView::disableStartButton()
{
    _ui.startButton->setEnabled(false);
}

QString PongServerView::status() const
{
    return _ui.statusText->toPlainText();
}


void PongServerView::startClickedSlot()
{
    emit startClickedSignal();
}
