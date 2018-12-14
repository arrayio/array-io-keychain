#ifndef PASSWORDENTERELEMENT_H
#define PASSWORDENTERELEMENT_H

#include <QWidget>
#include <QLabel>
#include <QStyle>
#include <QFrame>
#include  <QLineEdit>
#include <QRegexp>

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

private:
	//Ui::SecureWindowElement ui;
	QLabel *label = Q_NULLPTR;
	QLineEdit *value = Q_NULLPTR;
	QLabel *description;
	QLabel *labelConfirm = Q_NULLPTR;
	QLineEdit *valueConfirm = Q_NULLPTR;
	bool pCreatePassword;
	int _height = 0;
	int pValidChecks = 0;
	bool pStrong = false;

public slots:
	void checkStrength(const QString &text);
	void checkConfirm(const QString &text);
};

#endif