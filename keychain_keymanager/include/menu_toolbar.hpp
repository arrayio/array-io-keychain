#ifndef MENU_TOOLBAR_H
#define MENU_TOOLBAR_H

#include <QToolBar>
#include <QMenu>
#include <QAction>
#include <QTreeView>
#include <QTreeWidget>
#include <QPushButton>

class menu_toolbar : public QToolBar
{
    Q_OBJECT
public:
    menu_toolbar(QWidget *parent = Q_NULLPTR);

private slots:
    void Export();
    void Import();
    void Exit();
    void About();
    void Status();
};

#endif // ! MENU_TOOLBAR_H

