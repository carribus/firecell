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
#include <string>
#include <fstream>
#include <vector>
#include "../../common/XML/irrXML.h"
#include "../../common/CmdLineInfo.h"

using namespace std;
using namespace irr;
using namespace io;

#define DEFNAME_LENGTH	35

bool g_ExportWideStrings = false;

void print_title()
{
	printf("FireCell Client String header file generator\n\n");
}

void print_usage()
{
	printf("usage: genstringh [options] <source string xml file> <target .h file>\n\n"); 
  printf("Options:\n");
  printf("\t-wide\t- export the strings as wide character strings\n\n");
}

////////////////////////////////////////////////////////////////////////////////////

void write_h_header(ofstream& f, string& src_filename)
{
	f << "/*\n"
		<< " * FireCell String Definition file\n"
		<< " *\n"
		<< " * Do not modify this file. It was generated from [" << src_filename << "] by genstringh\n */\n\n";
#ifdef _DEBUG
	f << "/*\n"
		<< "01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789\n"
		<< "*/\n\n";
#endif//_DEBUG
}

////////////////////////////////////////////////////////////////////////////////////

void write_h_stringdef(ofstream& f, string& string_id)
{
	size_t len = string_id.length();
	f << "#define STR_" << string_id << "\t";
	for ( size_t i = len; i < DEFNAME_LENGTH; i++ )
		f << " ";
  if ( g_ExportWideStrings )
    f << "L\"";
  else 
    f << "\"";
	f << string_id << "\"\n";
}

////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
	CCmdLineInfo cmdLine(argc, argv);
	IrrXMLReader* pXML = NULL;
	vector<string> arrStrings;

	print_title();
	if ( cmdLine.GetArgCount() < 2 )
	{
		print_usage();
		return 1;
	}

  string option;
	string src_file = cmdLine.GetArg(1);
	string target_file = cmdLine.GetArg(2);

  if ( src_file[0] == '-' )
  {
    option = src_file;
    src_file = cmdLine.GetArg(2);
    target_file = cmdLine.GetArg(3);

    if ( option == "-wide" )
    {
      g_ExportWideStrings= true;
      printf("[CONFIG] Exporting wide strings\n");
    }
  }

	pXML = createIrrXMLReader( src_file.c_str() );

	if ( pXML )
	{
		string elemName, string_id, string_val;

		while ( pXML->read() )
		{
			switch ( pXML->getNodeType() )
			{
			case	EXN_ELEMENT:
				elemName = pXML->getNodeName();
				if ( elemName == "String" )
				{
					string_id = pXML->getAttributeValue( "id" );
					arrStrings.push_back(string_id);
				}
				break;

			default:
				break;
			}
		}

		ofstream fs_target;

		fs_target.open( target_file.c_str() );
		if ( fs_target.is_open() )
		{
			write_h_header(fs_target, src_file);
			vector<string>::iterator it = arrStrings.begin();

			while ( it != arrStrings.end() )
			{
				write_h_stringdef(fs_target, *it);
				it++;
			}
		}
		fs_target.close();
	}

	printf("%ld string definitions written to %s\n", arrStrings.size(), target_file.c_str());

	return 0;
}