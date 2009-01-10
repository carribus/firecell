/*
    FireCell Server - The server code for the firecell multiplayer game
    Copyright (C) 2008  Peter M. Mares

		Contact: carribus@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
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