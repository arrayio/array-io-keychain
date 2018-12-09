#ifndef PRIVATEKEYINMEMORY_H
#define PRIVATEKEYINMEMORY_H

#include <QWidget>
#include "Transaction.h"
#include "SecureWindowElement.h"
#include "KeychainWidget.h"

class PrivateKeyInMemory : public KeychainWidget
{
	Q_OBJECT
public:
	PrivateKeyInMemory(const Transaction &transaction, QWidget * parent = Q_NULLPTR);
	void SetPosition(int x, int y, int width) override;
	int GetCurrentHeight() override;
	~PrivateKeyInMemory();

private:
	SecureWindowElement * unlockTime= Q_NULLPTR;
	int currentHeight = 0;
};

#endif


