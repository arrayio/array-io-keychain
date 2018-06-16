#ifndef SERVICELOGGER_H
#define SERVICELOGGER_H
#include <string>

using namespace std;

class ServiceLogger {
public:
	static ServiceLogger& getLogger()
	{
		static ServiceLogger instance;
		return instance;
	}
	void Log(std::string message);
};
#endif // SERVICELOGGER_H
