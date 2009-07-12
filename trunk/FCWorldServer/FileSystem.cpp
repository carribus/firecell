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
#include <algorithm>
#include <string>
#include "EventSystem.h"
#include "Event.h"
#include "FileSystem.h"
#include "FileSystemActionHandler.h"
#include "Player.h"

using namespace std;

DEFINE_EVENT_SOURCE(FileSystem);
DEFINE_EVENT(FileSystem, FileListing);
DEFINE_EVENT(FileSystem, ChangeDir);
DEFINE_EVENT(FileSystem, OSVersion);

FileSystem::FileSystem(FCObject* parent)
: FCObject("FileSystem", parent)
, m_pComputer(NULL)
, m_pCurrentDir(NULL)
, m_bCaseSensitive(false)
{
  m_root.filetype = File::FT_Directory;
  m_root.is_mutable = false;
}

////////////////////////////////////////////////////////////////////////

FileSystem::~FileSystem(void)
{
}

////////////////////////////////////////////////////////////////////////

bool FileSystem::LoadFromXML(const string& filename)
{
  bool bResult = false;

  if ( m_xmlDoc.LoadFile( filename ) )
  {
    bResult = ParseXML(m_xmlDoc);
  }

  return bResult;
}

////////////////////////////////////////////////////////////////////////

size_t FileSystem::EnumerateFiles(std::string path, vector<FileSystem::File>& target)
{
  size_t pos = path.find_first_of(m_dirSeperator), lastPos = 0;
  string strSubDir;
  File* pDir = NULL;

  target.erase( target.begin(), target.end() );

  if ( path[ path.length()-1 ] != m_dirSeperator[0] )
    path.append( m_dirSeperator );
 
  if ( !m_bCaseSensitive )
    std::transform(path.begin()+1, path.end(), path.begin()+1, (int(*)(int))tolower);

  while ( pos != string::npos )
  {
    strSubDir = path.substr(lastPos, pos - lastPos+1);
    if ( !pDir )
    {
      if ( strSubDir == m_root.filename )
        pDir = &m_root;
    }
    else
    {
      strSubDir.erase( strSubDir.length()-1, 1 );
      map<string, File>::iterator it = pDir->files.find(strSubDir);

      if ( it != pDir->files.end() )
      {
        File& f = it->second;
        pDir = &f;
      }
      else
      {
        pDir = NULL;
        break;
      }
    }
    lastPos = pos+1;
    pos = path.find_first_of(m_dirSeperator, lastPos);
  }

  // check if the correct path was passed in to us
  if ( pDir )
  {
    map<string, File>::iterator it = NULL;
    File f;

    for ( it = pDir->files.begin(); it != pDir->files.end(); it++ )
    {
      f = it->second;
      target.push_back(f);
    }
  }

  return target.size();
}

////////////////////////////////////////////////////////////////////////

bool FileSystem::CanHandleCommand(const std::string& cmd)
{
  CommandMap::iterator it = m_mapCommands.find(cmd);

  return ( it != m_mapCommands.end() );
}

////////////////////////////////////////////////////////////////////////

bool FileSystem::ExecuteCommand(Player* pCaller, const std::string& cmd, const std::string& arguments, std::string& result)
{
  if ( cmd.length() == 0 )
    return "";

  bool ret = false;
  string action, cmdName;
  CommandMap::iterator it = m_mapCommands.find(cmd);

  if ( it != m_mapCommands.end() )
  {
    cmdName = it->second.name;
    action = it->second.action;

    if ( action == "FileListing" )
    {
      result = FileSystemActionHandler::Action_FileListing(this, arguments);
      ret = true;
      // emit an event to the caller object
      EventSystem::GetInstance()->Emit( this, static_cast<IEventTarget*>(pCaller), new Event(FileSystem::EVT_FileListing, NULL, pCaller) );
    }
    else if ( action == "ChangeDir" )
    {
      result = FileSystemActionHandler::Action_ChangeDirectory(this, arguments);
      ret = true;
      // emit an event to the caller object
      EventSystem::GetInstance()->Emit( this, static_cast<IEventTarget*>(pCaller), new Event(FileSystem::EVT_ChangeDir, NULL, pCaller) );
    }
    else if ( action == "OSVersion" )
    {
      result = FileSystemActionHandler::Action_OSVersion(this, arguments);
      ret = true;
      // emit an event to the caller object
      EventSystem::GetInstance()->Emit( this, static_cast<IEventTarget*>(pCaller), new Event(FileSystem::EVT_OSVersion, NULL, pCaller) );
    }
    else
    {
      result = "Unrecognised command\n\n";
    }
  }
  else
    result = "Unrecognised command\n\n";

  return ret;
}

////////////////////////////////////////////////////////////////////////

bool FileSystem::IsRootDir(const string& path)
{
  if ( m_root.filename == path )
    return true;
  return false;
}

////////////////////////////////////////////////////////////////////////

string FileSystem::GetCurrentPathName()
{
  if ( !m_pCurrentDir )
    return "Path Error: No Current path!";

  File* pDir = m_pCurrentDir;
  string pathName;
  list<File*> recursionStack;

  while ( pDir->parent )
  {
    recursionStack.push_front(pDir);
    pDir = pDir->parent;
  }
  recursionStack.push_front(pDir);

  // build the path name
  list<File*>::iterator it = recursionStack.begin();

  while ( it != recursionStack.end() )
  {
    pathName += (*it)->filename;
    if ( (*it)->parent )
      pathName += m_dirSeperator;
    it++;
  }

  return pathName;
}

////////////////////////////////////////////////////////////////////////

bool FileSystem::SetCurrentDir(string path)
{
  string tempPath = path;
  size_t pos = path.find_first_of(m_dirSeperator), lastPos = 0;
  string strSubDir;
  File* pDir = NULL;

  if ( path[ path.length()-1 ] != m_dirSeperator[0] )
    path.append( m_dirSeperator );

  if ( !m_bCaseSensitive )
    std::transform(path.begin()+1, path.end(), path.begin()+1, (int(*)(int))tolower);

  while ( pos != string::npos )
  {
    strSubDir = path.substr(lastPos, pos - lastPos+1);
    if ( !pDir )
    {
      if ( strSubDir == m_root.filename )
        pDir = &m_root;
      else
        break;
    }
    else
    {
      strSubDir.erase( strSubDir.length()-1, 1 );

      map<string, File>::iterator it = pDir->files.find(strSubDir);

      if ( it != pDir->files.end() )
      {
        File& f = it->second;
        pDir = &f;
      }
      else
      {
        if ( strSubDir == ".." )
        {
          pDir = pDir->parent;
        }
        else
        {
          pDir = NULL;
          break;
        }
      }
    }
    lastPos = pos+1;
    pos = path.find_first_of(m_dirSeperator, lastPos);
  }

  if ( pDir )
  {
    m_currentPath = tempPath;
    m_pCurrentDir = pDir;

    return true;
  }

  return false;
}

////////////////////////////////////////////////////////////////////////

bool FileSystem::ParseXML(TiXmlDocument& xml)
{
  bool bResult = true;
  TiXmlElement* pRoot = xml.FirstChildElement("FCFileSystem");

  // check if we have the root of the file system
  if ( pRoot )
  {
    if ( (bResult = ParseElement_FileSystem(pRoot)) )
    {
      // set the current directory to the root
      m_pCurrentDir = &m_root;
      // parse the files
      if ( (bResult = ParseElements_Files(pRoot)) )
      {
        bResult = ParseElements_Commands(pRoot);
      }
    }

    m_currentPath = m_root.filename;
  }
  else
    bResult = false;

  return bResult;
}

////////////////////////////////////////////////////////////////////////

bool FileSystem::ParseElement_FileSystem(TiXmlElement* pElem)
{
  bool bResult = true;
  string attrName, value;
  TiXmlAttribute* pAttr = pElem->FirstAttribute();

  // configure the FileSystem properties
  while ( pAttr )
  {
    attrName = pAttr->Name();
    value = pAttr->Value();

    if ( attrName == "owner" )
    {
      if (value == "Player")
        m_ownerType = FileSystem::FSO_Player;
      else
        m_ownerType = FileSystem::FSO_NPC;
    }
    else if ( attrName == "id" )
    {
      // do nothing, our owner has the id already
    }
    else if ( attrName == "style" )
    {
      if ( value == "unix" )
      {
        m_style = FileSystem::FSS_Unix;
        m_dirSeperator = "/";
        m_root.filename = m_dirSeperator;
      }
      else
      {
        m_style = FileSystem::FSS_Windows;
        m_dirSeperator = "\\";
        m_pathPrefix = "C:";
        m_root.filename = m_pathPrefix + m_dirSeperator;
      }
    }
    else if ( attrName == "casesensitive" )
    {
      m_bCaseSensitive = !(value == "0");
    }

    pAttr = pAttr->Next();
  }

  return bResult;
}

////////////////////////////////////////////////////////////////////////

bool FileSystem::ParseElements_Commands(TiXmlElement* pFSElem)
{
  bool bResult = true;
  TiXmlElement* pCmdElem = pFSElem->FirstChildElement("Command");

  // Look for all Command elements
  while ( pCmdElem )
  {
    if ( !(bResult = ParseElement_Command(pCmdElem)) )
      break;
    pCmdElem = pCmdElem->NextSiblingElement("Command");
  }

  return bResult;
}

////////////////////////////////////////////////////////////////////////

bool FileSystem::ParseElement_Command(TiXmlElement* pCmdElem)
{
  bool bResult = true;
  string attrName, value;
  Command cmd;
  TiXmlAttribute* pAttr = pCmdElem->FirstAttribute();

  while ( pAttr )
  {
    attrName = pAttr->Name();
    value = pAttr->Value();

    if ( attrName == "name" )
    {
      cmd.name = value;
    }
    else if ( attrName == "action" )
    {
      cmd.action = value;
    }

    pAttr = pAttr->Next();
  }
  m_mapCommands[ cmd.name ] = cmd;

  return bResult;
}

////////////////////////////////////////////////////////////////////////

bool FileSystem::ParseElements_Files(TiXmlElement* pParent)
{
  bool bResult = true;

  TiXmlElement* pFileElem = pParent->FirstChildElement("File");

  // Look for all File elements
  while ( pFileElem )
  {
    if ( !(bResult = ParseElement_File( pFileElem )) )
      break;
    pFileElem = pFileElem->NextSiblingElement("File");
  }

  return bResult;
}

////////////////////////////////////////////////////////////////////////

bool FileSystem::ParseElement_File(TiXmlElement* pFileElem)
{
  bool bResult = true;
  string attrName, value, keyname;
  File file;
  TiXmlAttribute* pAttr = pFileElem->FirstAttribute();

  // read the current file's attributes
  while ( pAttr )
  {
    attrName = pAttr->Name();
    value = pAttr->Value();

    if ( attrName == "type" )
    {
      if ( value == "dir" )
        file.filetype = File::FT_Directory;
      else
        file.filetype = File::FT_File;
    }
    else if ( attrName == "name" )
    {
      file.filename = value;
    }
    else if ( attrName == "mutable" )
    {
      file.is_mutable = (value == "1" || value == "true");
    }
    pAttr = pAttr->Next();
  }

  file.parent = m_pCurrentDir;

  // map the file
  keyname = file.filename;

  if ( !m_bCaseSensitive )
    std::transform(keyname.begin(), keyname.end(), keyname.begin(), (int(*)(int))tolower);

  m_pCurrentDir->files[ keyname ] = file;

  // Now that we have a file, lets check if its a directory, and if it is, we recurse into the child elements to populate it
  if ( file.filetype == File::FT_Directory && !pFileElem->NoChildren() )
  {
    m_pCurrentDir = &m_pCurrentDir->files[keyname];
    bResult = ParseElements_Files( pFileElem );
    m_pCurrentDir = file.parent;
  }

  return bResult;
}