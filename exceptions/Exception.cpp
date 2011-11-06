#include "Exception.h"

/**
 * Default constructor.
 *
 * @param code Exception code
 * @param message Exceptions message, passed to runtime_error
 * @param origin Method which has thrown the exception
 * @param severity Severity of exception
 */
Exception::Exception(int code, const string& message, const string& origin, Severity severity)
	: runtime_error(message), code_(code), origin_(origin), severity_(severity)
{
}

int Exception::getErrorCode() const
{
	return code_;
}

Severity Exception::getSeverity() const
{
	return severity_;
}
