#ifndef HINTS_TOOLBAR_H
#define HINTS_TOOLBAR_H

#include <QToolBar>
#include <QMenu>
#include <QAction>
#include <QTreeView>
#include <QTreeWidget>
#include <QPushButton>
#include <QApplication>
#include "custom_qlabel.hpp"

class hints_toolbar : public QToolBar
{
    Q_OBJECT
public:
    hints_toolbar(QWidget *parent = Q_NULLPTR);

private slots:
    void Minimize();
    void CloseWindow();

signals:
    void MinimizeSelected(QString text);
    void CloseWindowSelected(QString text);
};

#endif // ! HINTS_TOOLBAR_H