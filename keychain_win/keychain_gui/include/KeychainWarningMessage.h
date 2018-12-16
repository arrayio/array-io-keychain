#ifndef KEYCHAINWARNINGMESSAGE_H
#define KEYCHAINWARNINGMESSAGE_H

#include <QString>
#include <QStack>

class KeychainWarningMessage {
public:
	enum WarningType {
		NoWarnig = 0,
		UnlockWarning = 1,
		HashWarnig =2,
		FailedWarning = 3,
		CreateWarning = 4
	};

public:
	KeychainWarningMessage() {
		_warnTypes = new QStack<int>();
		_messages[0] = "This transaction is successfully parsed by the core module. No threats detected. You can now review the transaction details.";
		_messages[1] = "Your private key is encrypted and secure.";
		_messages[2] = "Experimental function 'unlock key' has been activated. During this time KeyChain will be able to sign transactions without the user confirmation.";
		_messages[3] = "Using transaction hash does not provide any information about the transaction. Use the transaction hash only if you trust the app requesting the signature.";
		_messages[4] = "KeyChain can provide only hex view of the transaction without additional information, such as address, amount, and any other additional information. Confim the transaction only if you trust the app requesting the signature.";
		
	}

	void SetWarning(WarningType warning) {
		_warnTypes->push((int)warning);
		//_warnType = warning;
	}

	QString GetMessage() const {
		QString _message("");
		while (!_warnTypes->isEmpty()) {
			int _mIndex = (int)_warnTypes->pop();
			_message += _messages[_mIndex];
			_message += '\n';
		}
		return _message;
	}

	bool isWarn() const {
		if (_warnType >=2)
			return true;
		return false;
	}

private:
	QString _messages[5];
	QStack<int> * _warnTypes;
	WarningType _warnType = WarningType::NoWarnig;
};

#endif