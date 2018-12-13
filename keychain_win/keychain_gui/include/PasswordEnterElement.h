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
	PasswordEnterElement(bool withDescription = false, QWidget * parent=Q_NULLPTR);
	~PasswordEnterElement();
	void SetPosition(int x, int y, int valueWidth);
	void SetLabel(QString label);
	bool IsValid();
	void SetLabelStyle(QString style);
	void SetValueStyle(QString style);
	QString GetValue();
	int GetElementHeigth();

private:
	//Ui::SecureWindowElement ui;
	QLabel *label;
	QLineEdit *value;
	QLabel *description;
	bool pWithDescription;
	int _height = 0;
	int pValidChecks = 0;
	bool pStrong = false;
};

#endif