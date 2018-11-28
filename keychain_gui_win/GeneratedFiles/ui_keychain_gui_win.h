/********************************************************************************
** Form generated from reading UI file 'keychain_gui_win.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KEYCHAIN_GUI_WIN_H
#define UI_KEYCHAIN_GUI_WIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_keychain_gui_winClass
{
public:

    void setupUi(QDialog *keychain_gui_winClass)
    {
        if (keychain_gui_winClass->objectName().isEmpty())
            keychain_gui_winClass->setObjectName(QStringLiteral("keychain_gui_winClass"));
        keychain_gui_winClass->resize(600, 400);

        retranslateUi(keychain_gui_winClass);

        QMetaObject::connectSlotsByName(keychain_gui_winClass);
    } // setupUi

    void retranslateUi(QDialog *keychain_gui_winClass)
    {
        keychain_gui_winClass->setWindowTitle(QApplication::translate("keychain_gui_winClass", "keychain_gui_win", nullptr));
    } // retranslateUi

};

namespace Ui {
    class keychain_gui_winClass: public Ui_keychain_gui_winClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KEYCHAIN_GUI_WIN_H
