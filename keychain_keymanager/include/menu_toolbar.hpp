#ifndef MENU_TOOLBAR_H
#define MENU_TOOLBAR_H

#include <QToolBar>
#include <QMenu>
#include <QAction>
#include <QTreeView>
#include <QTreeWidget>
#include <QPushButton>
#include <QApplication>
#include <QLabel>

class menu_toolbar : public QToolBar
{
    Q_OBJECT
public:
    menu_toolbar(QWidget *parent = Q_NULLPTR);

private slots:
    void Export();
    void Import();
    void About();
    void Status();
    void Exit();

signals:
    void ExportSelected(QString text);
    void ImportSelected(QString text);
    void AboutSelected(QString text);
    void StatusSelected(QString text);
    void ExitSelected(QString text);
};

#endif // ! MENU_TOOLBAR_H

