#ifndef PASSWORDENTERELEMENT_H
#define PASSWORDENTERELEMENT_H

#include <QWidget>
#include <QLabel>
#include <QStyle>
#include <QFrame>
#include  <QLineEdit>
//#include <QRegexp>
#include "PasswordLineEdit.h"
#include "CheckPasswordStrength.h"

class PasswordEnterElement : public QWidget
{
	Q_OBJECT
public:
	PasswordEnterElement(bool passwordCreate = false, QWidget * parent=Q_NULLPTR);
	~PasswordEnterElement();
	void SetPosition(int x, int y, int valueWidth);
	void SetLabel(QString label);
	void SetLabelStyle(QString style);
	void SetValueStyle(QString style);
	QString GetValue();
	int GetElementHeigth();
	void setValueFocus();
	void checkFinishedEnterance();
	void setFocusOnConfirm();
	bool validConfirm();
    PasswordLineEdit *value = Q_NULLPTR;
    PasswordLineEdit *valueConfirm = Q_NULLPTR;
	void tabFocusByMouse (int line);

signals:
	void finishEnterPassword();
    void focus (int line);

private:
	//Ui::SecureWindowElement ui;
	QLabel *label = Q_NULLPTR;

	QLabel *description;
	QLabel *labelConfirm = Q_NULLPTR;
	QLabel *confirmDescription;

	bool pCreatePassword;
	int _height = 0;
	int pValidChecks = 0;
	bool pStrong = false;
	bool isSame = false;

public slots:
	void checkStrength(bool stregth);
	void checkConfirm(const bool confirm);
};

#endif