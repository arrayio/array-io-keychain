#ifndef CHECKPASSWORDSTRENGTH_H
#define CHECKPASSWORDSTRENGTH_H


class CheckPasswordStrength {
public:
    enum struct PasswordStrength {
        unknown=0,
        weak,
        middle,
        strong,
        last
    };
public:
	CheckPasswordStrength();
	~CheckPasswordStrength();
	PasswordStrength check();
};

#endif