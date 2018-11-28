/********************************************************************************
** Form generated from reading UI file 'SecureWindowElement.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SECUREWINDOWELEMENT_H
#define UI_SECUREWINDOWELEMENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SecureWindowElement
{
public:

    void setupUi(QWidget *SecureWindowElement)
    {
        if (SecureWindowElement->objectName().isEmpty())
            SecureWindowElement->setObjectName(QStringLiteral("SecureWindowElement"));
        SecureWindowElement->resize(400, 300);

        retranslateUi(SecureWindowElement);

        QMetaObject::connectSlotsByName(SecureWindowElement);
    } // setupUi

    void retranslateUi(QWidget *SecureWindowElement)
    {
        SecureWindowElement->setWindowTitle(QApplication::translate("SecureWindowElement", "SecureWindowElement", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SecureWindowElement: public Ui_SecureWindowElement {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SECUREWINDOWELEMENT_H
