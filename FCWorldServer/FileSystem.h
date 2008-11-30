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
#ifndef _FILESYSTEM_H_
#define _FILESYSTEM_H_

#include <string>
#include <list>
#include <map>
#include <vector>
#include "../common/XML/irrXML.h"

using namespace irr;
using namespace io;

using namespace std;

// forward declaration(s)
class Computer;

class FileSystem
{
public:
  
  class Command
  {
  public:
    string name;
    string action;
  };

  class File
  {
  public:

    File() : parent(NULL), filetype(FT_Directory) {}

    enum FileType
    {
      FT_Directory,
      FT_File
    };

    string filename;
    FileType filetype;
    bool is_mutable;
    File* parent;

    map<string, File> files;
  };

  enum FSStyle
  {
    FSS_Unix,
    FSS_Windows
  };

  enum FSOwnerType
  {
    FSO_Player = 1,
    FSO_NPC,
    FSO_Company
  };

  FileSystem(void);
  ~FileSystem(void);

  bool LoadFromXML(const string& filename);
  size_t EnumerateFiles(string path, vector<FileSystem::File>& target);
  string ExecuteCommand(const string& cmd, const string& arguments);

  bool IsRootDir(const string& path);
  string GetCurrentPathName();
//  string GetCurrentPathName()                               { return m_currentPath; }
  bool SetCurrentDir(string path);
  FileSystem::File GetCurrentDir()                          { return *m_pCurrentDir; }

  FSStyle             GetFSStyle()                          { return m_style; }
  string              GetDirSeperator()                     { return m_dirSeperator; }
  string              GetPathPrefix()                       { return m_pathPrefix; }
  void                SetComputer(Computer* pComputer)      { m_pComputer = pComputer; }
  Computer*           GetComputer()                         { return m_pComputer; }
  bool                IsCaseSensitive()                     { return m_bCaseSensitive; }

private:

  /*
   *  Private methods
   */

  //
  // Parsing methods
  bool ParseXML(IrrXMLReader* pXML);
    void ParseElement_FileSystem(IrrXMLReader* pXML);
    void ParseElement_File(IrrXMLReader* pXML);
    void ParseElement_Command(IrrXMLReader* pXML);

  /*
   *  Private data
   */
  Computer*           m_pComputer;
  IrrXMLReader*       m_xml;
  FSStyle             m_style;
  FSOwnerType         m_ownerType;
  bool                m_bCaseSensitive;

  File                m_root;
  File*               m_pCurrentDir;
  list<File*>         m_recursionStack;
  string              m_dirSeperator;
  string              m_pathPrefix;
  string              m_currentPath;

  //
  // Command handling
  typedef map<string, Command> CommandMap;
  CommandMap          m_mapCommands;
};

#endif//_FILESYSTEM_H_