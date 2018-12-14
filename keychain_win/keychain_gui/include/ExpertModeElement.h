#ifndef EXPERTMODELEMENT_H
#define EXPERTMODELEMENT_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "ExpertModeDialog.h"
#include "ExpertModeLabel.h"

class ExpertModeElement : public QWidget
{
	Q_OBJECT

public:
	ExpertModeElement(QWidget* parent = Q_NULLPTR);
	~ExpertModeElement();
	void SetExpertModeText(QString expertMode);
	void SetPosition(int x, int y, int labelWidth, int valueWidth);

private:
	QLabel * expertLabel;
	ExpertModeLabel * expertValue;
	QPushButton * moreButton;

	QString expertModeText;

public slots:
	void showMoreForExpert();
};

#endif