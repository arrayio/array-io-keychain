#ifndef EXPERTMODE_DIALOG_H
#define EXPERTMODE_DIALOG_H

#include <QWidget>
#include <QDialog>
#include <QString>
#include <QPushButton>
#include <QPlainTextEdit>
#include <KeychainWidget.h>

class ExpertModeDialog : public QDialog
{
	Q_OBJECT

public:
	ExpertModeDialog(KeychainWidget * widget,  QWidget * parent = Q_NULLPTR);
	~ExpertModeDialog();
	void SetExpertModeText(QString string);

protected:
	void keyPressEvent(QKeyEvent *event) override; 
private: 
	QPlainTextEdit * transactionDescription;
	QPushButton * OKButton;

public slots:
	void closeExpertMode();
};

#endif