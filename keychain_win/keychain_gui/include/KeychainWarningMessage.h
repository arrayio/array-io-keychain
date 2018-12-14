#ifndef KEYCHAINWARNINGMESSAGE_H
#define KEYCHAINWARNINGMESSAGE_H

#include <QString>

class KeychainWarningMessage {
public:
	enum WarningType {
		NoWarnig = 0,
		HexWarning = 1,
		HashWarnig =2,
		FailedWarning = 3
	};

public:
	KeychainWarningMessage() {
		_messages[0] = "This transaction is verified. Your private keys are kept in an isolated environment to ensure a strong layer of protection.";
		_messages[1] = "Experimental function 'unlock key' has been activated. During this time KeyChain will be able to sign transactions without the user confirmation.";
		_messages[2] = "Using transaction hash does not provide any information about the transaction. Use the transaction hash only if you trust the app requesting the signature.";
		_messages[3] = "KeyChain can provide only hex view of the transaction without additional information, such as address, amount, and any other additional information. Confim the transaction only if you trust the app requesting the signature.";
	}

	void SetWarning(WarningType warning) {
		_warnType = warning;
	}

	QString GetMessage() const {
		return _messages[_warnType];
	}

	bool isWarn() const {
		if (_warnType > 0)
			return true;
		return false;
	}

private:
	QString _messages[4]; 
	WarningType _warnType = WarningType::NoWarnig;
};

#endif