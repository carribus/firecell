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
#include <windows.h>
#include "../../common/CmdLineInfo.h"
#include "../../common/database/FCDatabase.h"

#define DEFNAME_LENGTH	35

////////////////////////////////////////////////////////////////////////////////////

void print_title()
{
	printf("FireCell Software Types header file generator\n\n");
}

////////////////////////////////////////////////////////////////////////////////////

void print_usage()
{
	printf("usage: genswtypesh <database host> <database name> <username> <password> <target .h file>\n\n"); 
}

////////////////////////////////////////////////////////////////////////////////////

void write_h_header(ofstream& f)
{
	f << "/*\n"
		<< " * FireCell Sofware Types Definition file\n"
		<< " *\n"
		<< " * Do not modify this file.*/\n\n";
#ifdef _DEBUG
	f << "/*\n"
		<< "01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789\n"
		<< "*/\n\n";
#endif//_DEBUG
}

////////////////////////////////////////////////////////////////////////////////////

void write_h_swtypedef(ofstream& f, string& string_id, FCULONG id)
{
	size_t len = string_id.length();
	f << "#define " << string_id;
	for ( size_t i = len; i < DEFNAME_LENGTH; i++ )
		f << " ";
	f << id << "\n";
}

////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
	CCmdLineInfo cmdLine(argc, argv);
	string server, dbname, user, password, target_file;
	vector<string> arrStrings;
	FCDatabase db;
	FCDBJob job;

	print_title();
	if ( cmdLine.GetArgCount() < 5 )
	{
		print_usage();
		return 1;
	}

	server = cmdLine.GetArg(1);
	dbname = cmdLine.GetArg(2);
	user = cmdLine.GetArg(3);
	password = cmdLine.GetArg(4);
	target_file = cmdLine.GetArg(5);

	printf("Opening connection to database [%s] on server [%s]... \n", dbname.c_str(), server.c_str());
	db.Initialise("mysql", server, dbname, user, password, 1);
	db.ExecuteDirectQuery("SELECT * FROM fc_softwaretypes", "SWT_TYPES", NULL);

	while ( db.GetCompletedJobCount() == 0 )
	{
		Sleep(250);
	}

	if ( db.GetNextCompletedJob(job) )
	{
		DBIResults* pResult = job.GetResults();
		DBIResultSet* pSet = pResult->GetNextResultSet();
		size_t rowCount = 0;
		FCULONG id;
		string name, defID;

		ofstream fs_target;

		fs_target.open( target_file.c_str() );
		if ( fs_target.is_open() )
		{
			write_h_header(fs_target);
			rowCount = pSet->GetRowCount();
			for ( size_t i = 0; i < rowCount; i++ )
			{
				id = pSet->GetULongValue("software_type_id", i);
				name = pSet->GetStringValue("name", i);
				defID = pSet->GetStringValue("identifier", i);

				write_h_swtypedef(fs_target, defID, id);
			}
			fs_target.close();
		}
		printf("%ld software type definitions written to %s\n", rowCount, target_file.c_str());
	}

	return 0;
}