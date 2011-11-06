#include "ConfigException.h"

ConfigException::ConfigException(ConfigErrorCode code, const string& message, const string& origin, Severity severity)
: Exception(code, message, origin, severity)
{
}

const string ConfigException::getMessage(void) const
{
	string msg;

	switch(code_)
	{
	case config_INT:
		msg = "Key '";
		msg+= what();
		msg+= "' is not INT";
		break;
	case config_KEY:
		msg = "Key '";
		msg+= what();
		msg+= "' doesn't exist";
		break;
	default:
		msg = what();
	}

	msg += " (" + origin_ +")";
	return msg;
}
