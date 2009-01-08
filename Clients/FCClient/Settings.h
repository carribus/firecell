#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <map>
#include <string>
#include "../../common/fctypes.h"
#include "../../common/XML/irrXML.h"

using namespace std;
using namespace irr;
using namespace io;

class Settings
{
	Settings(void);
	~Settings(void);

public:

	static Settings& instance();
	static void destroy();

	bool LoadSettings(string filename);
	string GetValue(const string& key, const string& attrib);

private:

	static Settings*					m_pThis;

	std::map<string, string>				m_mapKeyToValue;
};

#endif//_SETTINGS_H_