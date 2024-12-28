/********************************************************************************
** Form generated from reading UI file 'WindowPins.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WINDOWPINS_H
#define UI_WINDOWPINS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WindowPinsClass
{
public:

    void setupUi(QWidget *WindowPinsClass)
    {
        if (WindowPinsClass->objectName().isEmpty())
            WindowPinsClass->setObjectName(QString::fromUtf8("WindowPinsClass"));
        WindowPinsClass->resize(600, 400);

        retranslateUi(WindowPinsClass);

        QMetaObject::connectSlotsByName(WindowPinsClass);
    } // setupUi

    void retranslateUi(QWidget *WindowPinsClass)
    {
        WindowPinsClass->setWindowTitle(QCoreApplication::translate("WindowPinsClass", "WindowPins", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WindowPinsClass: public Ui_WindowPinsClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINDOWPINS_H
