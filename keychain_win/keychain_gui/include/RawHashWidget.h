#ifndef RAWHASHWIDGET_H
#define RAWHASHWIDGET_H

#include <QWidget>
#include "Transaction.h"
#include "SecureWindowElement.h"
#include "ExpertModeElement.h"
#include "PrivateKeyInMemoryWidget.h"
#include "KeychainWidget.h"

class RawHashWidget : public KeychainWidget
{
	Q_OBJECT
public:
  using signhash_event = keychain_app::secmod_commands::secmod_event
    <keychain_app::secmod_commands::events_te::sign_hash>::params_t;
	RawHashWidget(const signhash_event& signhash, QWidget * parent = Q_NULLPTR);
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
