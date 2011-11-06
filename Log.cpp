#include "Log.h"

/**
 * Wrapper for debug level.
 *
 * @param msg Message
 * @sa print()
 */
void Log::debug(const string& msg)
{
//	if(LOG_LEVEL > 0)
//		return;
	print(msg, DEBUG);
}

/**
 * Wrapper for info level.
 *
 * @param msg Message
 * @sa print()
 */
void Log::info(const string& msg)
{
	print(msg, INFO);
}

/**
 * Wrapper for error level.
 *
 * @param msg Message
 * @sa print()
 */
void Log::error(const string& msg)
{
	print(msg, ERROR);
}

/**
 * Wrapper for warning level.
 *
 * @param msg Message
 * @sa print()
 */
void Log::warn(const string& msg)
{
	print(msg, WARN);
}

/**
 * Output message from exception
 *
 * @param e Exception to log.
 * @sa print()
 * @sa Exception::getMessage()
 */
//void Log::exception(const Exception& e)
//{
//	print(e.getMessage(), e.getSeverity());
//}

/**
 * Prints message, prepended with relevant tag.
 *
 * @param msg Message
 * @param severity Severity of message
 */
void Log::print(const string msg, Severity severity)
{
	if(LOG_LEVEL > severity)
	{
		return;
	}
	string tag;
	switch(severity)
	{
	case DEBUG:
		tag = "<debug> ";
		break;
	case INFO:
		tag = "<info>  ";
		break;
	case WARN:
		tag = "<warn>  ";
		break;
	case ERROR:
		tag = "<error> ";
	default:
		return;
	}
	LOG_OUT << tag << msg << endl;
}
