#ifndef PASSWORDENTERELEMENT_H
#define PASSWORDENTERELEMENT_H

#include <QWidget>
#include <QLabel>
#include <QStyle>
#include <QFrame>
#include  <QLineEdit>
#include <QRegexp>
#include "PasswordLineEdit.h"
#include "CheckPasswordStrength.h"

class PasswordEnterElementBase : public QWidget
{
	Q_OBJECT
public:
  PasswordEnterElementBase(QWidget * parent=nullptr);
  virtual ~PasswordEnterElementBase();
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

signals:
	void finishEnterPassword();
	void changePassword();

protected:
	//Ui::SecureWindowElement ui;
	QLabel *label = Q_NULLPTR;
	PasswordLineEdit *value = Q_NULLPTR;
	QLabel *description;
	QLabel *labelConfirm = Q_NULLPTR;
	QLabel *confirmDescription;
	PasswordLineEdit *valueConfirm = Q_NULLPTR;
	bool pCreatePassword;
	int _height = 0;
	int pValidChecks = 0;
	bool pStrong = false;
	bool isSame = false;

public slots:
	void checkStrength(const QString &text);
	void checkConfirm(const QString &text);
};

class PasswordEnterElementCreate : public PasswordEnterElementBase
{
public:
  PasswordEnterElementCreate(QWidget * parent = nullptr);
  virtual ~PasswordEnterElementCreate();
};

class PasswordEnterElement : public PasswordEnterElementBase
{
public:
  PasswordEnterElement(QWidget * parent = nullptr);
  virtual ~PasswordEnterElement();
};


#endif