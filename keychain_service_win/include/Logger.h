#ifndef LOGGER_H
#define  LOGGER_H
#include <ServiceLogger.h>

class Logger {
private:
	static ServiceLogger* _logger;
	Logger();
public :
	static ServiceLogger * get() {
		if (!_logger)
			_logger = new ServiceLogger();
		return _logger;
	}
};

#endif // 
