#ifndef RAWHASHWIDGET_H
#define RAWHASHWIDGET_H

#include <QWidget>
#include "SecureWindowElement.h"
#include "ExpertModeElement.h"
#include "PrivateKeyInMemoryWidget.h"
#include "KeychainWidget.h"

class RawHashWidget : public KeychainWidget
{
	Q_OBJECT
public:
	RawHashWidget( QWidget * parent = Q_NULLPTR);
	void SetPosition(int x, int y, int width) override;
	int GetCurrentHeight() override;
	int GetCurrentWidth() override;
	~RawHashWidget();

private:
	SecureWindowElement * from = Q_NULLPTR;
	SecureWindowElement * hash = Q_NULLPTR;
	//ExpertModeElement * expertModeElement = Q_NULLPTR;
	PrivateKeyInMemory * unlockTime = Q_NULLPTR;
	int max_width = 0;
	int currentHeight = 0;
	int currentWidth = 0;
};

#endif
