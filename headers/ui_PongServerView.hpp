/********************************************************************************
** Form generated from reading UI file 'PongServerView.ui'
**
** Created: Fri Apr 12 15:44:16 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PONGSERVERVIEW_H
#define UI_PONGSERVERVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PonServerView
{
public:
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *quitButton;
    QPushButton *startButton;
    QLabel *label;
    QPlainTextEdit *statusText;

    void setupUi(QWidget *PonServerView)
    {
        if (PonServerView->objectName().isEmpty())
            PonServerView->setObjectName(QString::fromUtf8("PonServerView"));
        PonServerView->resize(519, 455);
        verticalLayout_2 = new QVBoxLayout(PonServerView);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        quitButton = new QPushButton(PonServerView);
        quitButton->setObjectName(QString::fromUtf8("quitButton"));
        quitButton->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_4->addWidget(quitButton);

        startButton = new QPushButton(PonServerView);
        startButton->setObjectName(QString::fromUtf8("startButton"));
        startButton->setEnabled(false);
        startButton->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_4->addWidget(startButton);


        verticalLayout_2->addLayout(horizontalLayout_4);

        label = new QLabel(PonServerView);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout_2->addWidget(label);

        statusText = new QPlainTextEdit(PonServerView);
        statusText->setObjectName(QString::fromUtf8("statusText"));

        verticalLayout_2->addWidget(statusText);


        retranslateUi(PonServerView);
        QObject::connect(startButton, SIGNAL(clicked()), PonServerView, SLOT(startClickedSlot()));
        QObject::connect(quitButton, SIGNAL(clicked()), PonServerView, SLOT(quitClickedSlot()));

        QMetaObject::connectSlotsByName(PonServerView);
    } // setupUi

    void retranslateUi(QWidget *PonServerView)
    {
        PonServerView->setWindowTitle(QApplication::translate("PonServerView", "PongServerView", 0, QApplication::UnicodeUTF8));
        quitButton->setText(QApplication::translate("PonServerView", "Quit", 0, QApplication::UnicodeUTF8));
        startButton->setText(QApplication::translate("PonServerView", "Start", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("PonServerView", "Status message", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PonServerView: public Ui_PonServerView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PONGSERVERVIEW_H
