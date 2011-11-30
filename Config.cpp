#include "Config.h"

/**
 * Initializes ::loaded_
 */
Config::Config()
{
    loaded_ = false;
}

/**
 * Does nothing.
 */
Config::~Config()
{
}

/**
 * Loads and parse file.
 * @param fname Path to configuration file.
 */
void Config::parseFile(const string fname)
{
    if(loaded_)
	cerr << "Config::parseFile: Attempt to rewrite configuration" << endl;

    ifstream file(fname.c_str());
    int l = 0;


    if(!file.is_open())
	cerr << "Config::parseFile: Couldn't open conf. file: " << fname << endl;

    //	string key, value;
    char buffer[BUFFER_LEN];
    while(!file.eof())
    {
	l++;

	file.getline(buffer, BUFFER_LEN);

	if(!parseLine(buffer))
	    clog << "Config::parseFile: skipping line " << l << endl;
    }
    file.close();
    loaded_ = true;
}

/**
 * Parses the line and adds it to ::options_.
 *
 * @param line Line to be parsed.
 * @return TRUE, if the line was succesfuly parsed and options were added.
 */
bool Config::parseLine(const string& line)
{
    size_t posEnd = line.find('#');
    size_t posEq = line.find('=');

    if((posEq == string::npos) || posEnd < posEq)
	return false;

    string left = trim(line.substr(0, posEq));
    string right = trim(line.substr(posEq + 1, posEnd - posEq - 1));

    if(left.empty() || right.empty())
	return false;

    clog << "Config: '" << left << "' = '" << right << "'" << endl;
    add(left, right);
    return true;
}

/**
 * Helper function to trim white space from string.
 *
 * @param str String to trim.
 * @return Trimmed string.
 */
string Config::trim(const string& str)
{
    size_t start = str.find_first_not_of(" \t\n\r");
    if(start == string::npos)
	return "";
    size_t end = str.find_last_not_of(" \t\n\r");

    return str.substr(start, end - start + 1);
}

/**
 * Adds configuration option.
 *
 * @param key Configuration key.
 * @param value Configuration value.
 */
void Config::add(const string key, const string value)
{
    options_[key] = value;
}

/**
 * Gets option and converts it to int.
 * Uses getString()
 *
 * @param key Option's name.
 * @return Integer of option's value.
 */
int Config::getInt(const string& key) const
{
    string val = getString(key);
    bool ok;
    int ret = strToInt(val, ok);

    if(!ok)
	cerr << "Config::getInt: failed for " << key << endl;

    return ret;
}

/**
 * Gets option as string.
 *
 * @param key Option's name.
 * @return Option's value.
 */
string Config::getString(const string& key) const
{
    map<string, string>::const_iterator it = options_.find(key);
    if(it == options_.end())
    {
	cerr << "Config::getString: " << key << "not found" << endl;
    }

    return(string) (*it).second;
}


/**
 * Gets option as boolean.
 * Accepts only 0.as false.
 *
 * @param key Option's name
 * @return Option's value
 */
bool Config::getBool(const string& key) const
{
    string val = getString(key);

    if(val[0] == '0')
    {
	return false;
    }

    return true;
}
