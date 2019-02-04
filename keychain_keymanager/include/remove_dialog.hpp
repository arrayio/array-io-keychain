#ifndef REMOVE_DIALOG_H
#define REMOVE_DIALOG_H

#include <QtGui>
#include <QDialog>
#include <QGlobalStatic>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include "custom_qlabel.hpp"

class remove_dialog : public QDialog
{
public:
    remove_dialog(QWidget *parent = Q_NULLPTR);
    void show_dialog(const QString& keyName);
    static remove_dialog* instance();

private:
    QDialog *subDialog;
    QPoint position;
    int pWidth = 0;
    int pHeight = 0;
    static remove_dialog* r_instance;
};

#endif