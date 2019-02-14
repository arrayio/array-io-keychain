#ifndef MENU_TOOLBAR_H
#define MENU_TOOLBAR_H

#include <QToolBar>
#include <QAction>
#include <QTreeView>
#include <QTreeWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QtWidgets>
#include <QApplication>
#include "custom_qlabel.hpp"
#include "custom_menu.hpp"
#include "menu_button.hpp"

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

private:
    menu_button *FileButton;
    menu_button *AboutButton;
    menu_button *KeyButton;
    custom_menu *menuFile;
    custom_menu *menuAbout;
    QAction *actionCreate;
    QAction *actionExport;
    QAction *actionImport;
    QAction *actionExit;
    QAction *actionAbout;
    QAction *actionStatus;

signals:
    void CreateSelected(const QString &text);
    void ExportSelected(const QString &text);
    void ImportSelected(const QString &text);
    void AboutSelected(const QString &text);
    void StatusSelected(const QString &text);
    void ExitSelected(const QString &text);

protected:
    void enterEvent(QEvent *e) override;
};

#endif // ! MENU_TOOLBAR_H

