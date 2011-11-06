#ifndef _CONFIG_H
#define	_CONFIG_H
#include <iostream>
#include <fstream>
#include <map>
#include "util.h"
#include "Log.h"
#include "Singleton.h"
#include "exceptions/ConfigException.h"
using namespace std;

#define BUFFER_LEN 1024 //< Maximum length of line

/**
 * Singleton for accessing configuration options, stored in ::options_ map.
 */
class Config : public Singleton<Config>
{
public:
	/// Load and parse given configuration file
	void parseFile(const string);

	//Get values of given key.
	// Could use templates, but would be overkill for my needs

	int getInt(const string) const;
	string getString(const string) const;
	friend class Singleton<Config>;
protected:
	Config();
	~Config();
	bool parseLine(const string&);
	string trim(const string&);

	//add key-value pair
	void add(const string, const string);

	/// Options storage
	map<string, string> options_;

	/// Whether config file was already loaded. To prevent multiple calls to parseFile()
	bool loaded_;
private:
	Config(const Config&);

};

#endif	/* _CONFIG_H */

