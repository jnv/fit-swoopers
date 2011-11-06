#ifndef _LOG_H
#define	_LOG_H
#include <iostream>
#include "Singleton.h"
#include "global.h"
using namespace std;

/**
 * Simple class for loging.
 * Works with Severity enum, verbosity can be changed by LOG_LEVEL.
 * Derived from HTTPD project.
 * @author Jan Vlnas
 */
class Log : public Singleton<Log>
{
public:
    void debug(const string&);
    void info(const string&);
    void warn(const string&);
    void error(const string&);

    //void exception(const Exception&);

    friend class Singleton<Log>;
protected:

    Log()
    {
    } //< Does nothing.

    ~Log()
    {
    } //< Does nothing.

    void print(const string, Severity = INFO);

    Log(const Log&)
    {
    };

};

#endif	/* _LOG_H */

