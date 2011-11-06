#ifndef _CONFIGEXCEPTION_H
#define	_CONFIGEXCEPTION_H
#include "Exception.h"

enum ConfigErrorCode
{
	config_FILE_OPEN,
	config_FILE_READ,
	config_KEY,
	config_INT,
	config_LOADED,
	config_OTHER
};

/**
 * Used by Config.
 * Any errors within configuration are considered critical.
 */
class ConfigException : public Exception
{
public:
	ConfigException(ConfigErrorCode, const string&, const string&, Severity = ERROR);
	virtual ~ConfigException() throw() {};
	const string getMessage(void) const;
private:

};

#endif	/* _CONFIGEXCEPTION_H */

