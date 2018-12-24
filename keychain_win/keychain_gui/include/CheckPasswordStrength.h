#ifndef CHECKPASSWORDSTRENGTH_H
#define CHECKPASSWORDSTRENGTH_H

#include <QString>
#include <QRegExp>

class CheckPasswordStrength {
public :
	enum PasswordStrength {
		weak=1,
		middle=2,
		strong=3
	};
public:
	CheckPasswordStrength();
	~CheckPasswordStrength();
	PasswordStrength check(QString password);
};

#endif