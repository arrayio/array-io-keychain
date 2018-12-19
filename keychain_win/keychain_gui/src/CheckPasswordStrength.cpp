#include "CheckPasswordStrength.h"

CheckPasswordStrength::CheckPasswordStrength()
{
}

CheckPasswordStrength::~CheckPasswordStrength()
{
}

CheckPasswordStrength::PasswordStrength CheckPasswordStrength::check(QString password)
{
	QRegExp regLetter("[a-zA-Z]+");
	QRegExp regDigit("[0-9]+");
	QRegExp regSymbol("[@#$%*]+");
	if (regSymbol.indexIn(password) != -1) {
		if (password.length() >= 8)
		{
			return PasswordStrength::strong;
		}
		return PasswordStrength::middle;
	}
	return PasswordStrength::weak;
}
