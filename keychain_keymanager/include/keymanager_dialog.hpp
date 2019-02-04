#ifndef  KEYMANAGERDIALOG_H
#define  KEYMANAGERDIALOG_H

#include <QtWidgets/QDialog>
#include <QObject>
#include <QWidget>
#include <QApplication>
#include <QLabel>
#include <QTreeWidget>
#include <QFontDatabase>
#include <QFont>
#include <QTextEdit>
#include <QWindowStateChangeEvent>
#include "keychain_table.hpp"
#include "menu_toolbar.hpp"
#include "hints_toolbar.hpp"
#include "key_scroll_area.hpp"

class keymanager_dialog : public QDialog
{
	Q_OBJECT
public:
	keymanager_dialog(QWidget *parent = Q_NULLPTR);
	void init();

private:
	QLabel *header;
	QLabel *logoLabel;
	QLabel *logoTitle;
    keychain_table *keysTable;
	QLabel *wList;
    menu_toolbar *toolbar;
	hints_toolbar *hintsbar;
    QTextEdit *textEditform;

public slots:
	//process toolbar menu actions
    void ProcessCreate(const QString &text);
    void ProcessExport(const QString &text);
    void ProcessImport(const QString &text);
    void ProcessAbout(const QString &text);
    void ProcessStatus(const QString &text);
    void ProcessExit(const QString &text);
	
	//process hintsbar actions
	void ProcessMinimize(const QString &text);
    
protected:
    //process hint events to control window behavior 
    virtual void changeEvent(QEvent *ev) override;
};

#endif // ! KEYMANAGERDIALOG_H

