#ifndef LOGGER_H_
#define LOGGER_H_

#include <iostream>

class Logger {
public:
	Logger(std::ostream& outputStream = std::cout);
	virtual ~Logger();

	template<typename T>
	Logger& operator<<(const T& dataToLog);

private:
	std::ostream& outputStream;
};

template<typename T>
Logger& Logger::operator<<(const T& dataToLog) {
	outputStream << dataToLog;
	return *this;
}

#endif /* LOGGER_H_ */
