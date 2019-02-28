#ifndef UNLOCKKEYWIDGET_H
#define UNLOCKKEYWIDGET_H

#include <QWidget>
#include "KeychainWidget.h"
#include "PrivateKeyInMemoryWidget.h"

class UnlockKeyWidget : public KeychainWidget
{
	Q_OBJECT
public:
	UnlockKeyWidget( QWidget *parent = Q_NULLPTR);
	void SetPosition(int x, int y, int width) override;
	int GetCurrentHeight() override;
	int GetCurrentWidth() override;
	~UnlockKeyWidget();

private:
	PrivateKeyInMemory * unlockTime = Q_NULLPTR;
	int currentHeight = 0;
	int currentWidth = 0;

};


#endif

