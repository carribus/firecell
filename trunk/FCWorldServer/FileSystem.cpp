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
#include "FileSystem.h"
#include "FileSystemActionHandler.h"

using namespace std;

FileSystem::FileSystem(void)
: m_pComputer(NULL)
, m_xml(NULL)
, m_pCurrentDir(NULL)
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
  string elemName, attrName, value;
  File* pCurrent = NULL;
  File file;
  list<File*> recursionStack;
  m_xml = createIrrXMLReader(filename.c_str());

  // Parse the XML
  if ( m_xml )
  {
    bResult = ParseXML(m_xml);
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

string FileSystem::ExecuteCommand(const std::string& cmd, const std::string& arguments)
{
  if ( cmd.length() == 0 )
    return "";

  string ret, action, cmdName;
  CommandMap::iterator it = m_mapCommands.find(cmd);

  if ( it != m_mapCommands.end() )
  {
    cmdName = it->second.name;
    action = it->second.action;

    if ( action == "FileListing" )
    {
      ret = FileSystemActionHandler::Action_FileListing(this, arguments);
    }
    else if ( action == "ChangeDir" )
    {
      ret = FileSystemActionHandler::Action_ChangeDirectory(this, arguments);
    }
    else if ( action == "OSVersion" )
    {
      ret = FileSystemActionHandler::Action_OSVersion(this, arguments);
    }
    else
    {
      ret = "Unrecognised command";
    }
  }
  else
    ret = "Unrecognised command";

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

bool FileSystem::ParseXML(IrrXMLReader* pXML)
{
  if ( !pXML )
    return false;

  string elemName, attrName, value;

  while ( pXML->read() )
  {
    switch ( pXML->getNodeType() )
    {
    case  EXN_ELEMENT:
      elemName = pXML->getNodeName();
      if ( elemName == "FCFileSystem" )
      {
        ParseElement_FileSystem(pXML);
      }
      else if ( elemName == "File" )
      {
        ParseElement_File(pXML);
      }
      else if ( elemName == "Command" )
      {
        ParseElement_Command(pXML);
      }
      break;

    case  EXN_ELEMENT_END:
      elemName = pXML->getNodeName();
      if ( elemName == "File" )
      {
        m_pCurrentDir = m_recursionStack.back();
        m_recursionStack.pop_back();
      }
      break;
    }
  }

  m_pCurrentDir = &m_root;
  m_currentPath = m_root.filename;

  return true;
}

////////////////////////////////////////////////////////////////////////

void FileSystem::ParseElement_FileSystem(IrrXMLReader* pXML)
{
  if ( !pXML )
    return;

  string attrName, value;

  // start reading the file structure
  // get the file system details
  for ( int i = 0; i < pXML->getAttributeCount(); i++ )
  {
    attrName = pXML->getAttributeName(i);
    value = pXML->getAttributeValue(attrName.c_str());
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
        m_root.filename = "C:" + m_dirSeperator;
      }
    }
  }

  m_pCurrentDir = &m_root;
}

////////////////////////////////////////////////////////////////////////

void FileSystem::ParseElement_File(IrrXMLReader* pXML)
{
  if ( !pXML )
    return;

  File file;
  string attrName, value;

  // get the file details
  for ( int i = 0; i < pXML->getAttributeCount(); i++ )
  {
    attrName = pXML->getAttributeName(i);
    value = pXML->getAttributeValue(attrName.c_str());
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
  }

  file.parent = m_pCurrentDir;

  m_pCurrentDir->files[ file.filename ] = file;
  if ( file.filetype == File::FT_Directory )
  {
    if ( !pXML->isEmptyElement() )
    {
      m_recursionStack.push_back(m_pCurrentDir);
      File& f = m_pCurrentDir->files[ file.filename ];
      m_pCurrentDir = &f;
    }
  }
}

////////////////////////////////////////////////////////////////////////

void FileSystem::ParseElement_Command(IrrXMLReader* pXML)
{
  if ( !pXML )
    return;

  Command cmd;
  string attrName, value;

  // get the command details
  for ( int i = 0; i < pXML->getAttributeCount(); i++ )
  {
    attrName = pXML->getAttributeName(i);
    value = pXML->getAttributeValue(attrName.c_str());
    if ( attrName == "name" )
    {
      cmd.name = value;
    }
    else if ( attrName == "action" )
    {
      cmd.action = value;
    }
  }

  m_mapCommands[ cmd.name ] = cmd;
}

////////////////////////////////////////////////////////////////////////
