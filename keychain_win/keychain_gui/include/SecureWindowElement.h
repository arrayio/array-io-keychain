#pragma once

#include <QWidget>
#include <QLabel>
#include <QFrame>
//#include "ui_SecureWindowElement.h"

class SecureWindowElement : public QWidget
{
	Q_OBJECT

public:
	SecureWindowElement(QWidget *parent = Q_NULLPTR);
	~SecureWindowElement();
	void SetPosition(int x, int y, int labelWidth, int valueWidth);
	void SetLabelOffset(int offset);
	void SetLabelAndValue(QString label);
	void SetLabelAndValue(QString label, QString value);
	void SetLabelStyle(QString style);
	void SetValueStyle(QString style);

private:
	//Ui::SecureWindowElement ui;
	QLabel *label;
	QLabel *value;
	int _labelX;
	int _valueX;
};
