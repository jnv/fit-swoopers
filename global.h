/**@file global.h
 * Contains useful global options.
 */

#ifndef _GLOBAL_H
#define	_GLOBAL_H

/// Corresponds to Severity
#define LOG_LEVEL DEBUG

/// Where the logs will be written to (clog by default)
#define LOG_OUT clog

/**
 * Used by Log and Exception
 */
enum Severity
{
	DEBUG,
	INFO,
	WARN,
	ERROR,
	DISABLED
};

#endif	/* _GLOBAL_H */

