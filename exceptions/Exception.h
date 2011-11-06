#ifndef _EXCEPTION_H
#define	_EXCEPTION_H
#include <string>
#include <stdexcept>
#include "../global.h"
using namespace std;


/**
 * Custom exception hierarchy, with more verbosity.
 * Adds support for exception codes, origin and Severity.
 */
class Exception : public runtime_error
{
public:
	explicit Exception(int, const string&, const string&, Severity = ERROR);

	virtual ~Exception() throw ()
	{
	};

	int getErrorCode(void) const;

	/**
	 * Must be implemeted by subclasses, useful for logging.
     * @return Exception - specificuser-friendly message.
     */
	virtual const string getMessage(void) const = 0;
	Severity getSeverity(void) const;
protected:
	int code_;
	string origin_;
	const Severity severity_;
private:
//	Exception(const Exception&){};
};

#endif	/* _EXCEPTION_H */

