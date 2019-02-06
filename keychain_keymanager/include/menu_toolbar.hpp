#ifndef MENU_TOOLBAR_H
#define MENU_TOOLBAR_H

#include <QToolBar>
#include <QAction>
#include <QTreeView>
#include <QTreeWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QApplication>
#include "custom_qlabel.hpp"
#include "custom_menu.hpp"

class menu_toolbar : public QToolBar
{
    Q_OBJECT
public:
    menu_toolbar(QWidget *parent = Q_NULLPTR);

private slots:
    void Create();
    void Export();
    void Import();
    void About();
    void Status();
    void Exit();

signals:
    void CreateSelected(const QString &text);
    void ExportSelected(const QString &text);
    void ImportSelected(const QString &text);
    void AboutSelected(const QString &text);
    void StatusSelected(const QString &text);
    void ExitSelected(const QString &text);
};

#endif // ! MENU_TOOLBAR_H

