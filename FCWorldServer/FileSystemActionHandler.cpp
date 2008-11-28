#include <sstream>
#include "FileSystemActionHandler.h"

FileSystemActionHandler::FileSystemActionHandler(void)
{
}

////////////////////////////////////////////////////////////////////////

FileSystemActionHandler::~FileSystemActionHandler(void)
{
}

////////////////////////////////////////////////////////////////////////

string FileSystemActionHandler::Action_FileListing(FileSystem* pFS, const string& arguments)
{
  string result = "";
  FileSystem::File curDir, f;
  string curPath;
  vector<FileSystem::File> files;

  if ( pFS )
  {
    stringstream ss;

    curDir = pFS->GetCurrentDir();
    curPath = pFS->GetCurrentPathName();

    if ( curDir.filetype == FileSystem::File::FT_Directory )
    {
      size_t numFiles = pFS->EnumerateFiles(curPath, files);

      ss << "File listing for " << curDir.filename << std::endl << std::endl;

      for ( size_t i = 0; i < numFiles; i++ )
      {
        f = files[i];
        ss << f.filename << "\t" << ( f.filetype == FileSystem::File::FT_Directory ? "<Dir>" : "     " ) << "\t" << ( f.is_mutable ? "--r--r--r" : "wrxwrxwrx" ) << std::endl;
      }

      ss << std::endl << (unsigned int)numFiles << " File(s)" << "\n\n";

      result = ss.str();
    }
  }
  else
  {
    result = "No filesystem associated with request.";
  }

  return result;
}

////////////////////////////////////////////////////////////////////////

string FileSystemActionHandler::Action_ChangeDirectory(FileSystem* pFS, const string& arguments)
{
  string result = "";
  FileSystem::File curDir, oldDir;
  size_t pos;

  if ( pFS )
  {
    string newDir;

    oldDir = curDir = pFS->GetCurrentDir();
    pos = arguments.find_first_of(pFS->GetDirSeperator());

    // setup the target path
    if ( pos == 0 && pos != std::string::npos )
    {
      // if the path is preceded with a directory seperator, we just take the whole argument as the new directory
      newDir = arguments;
    }
    else
    {
      newDir = (pFS->IsRootDir(curDir.filename) ? "" : pFS->GetCurrentPathName()) + pFS->GetDirSeperator() + arguments;
    }

    pFS->SetCurrentDir(newDir);
  }
  else
  {
    result = "No filesystem associated with request.";
  }

  return result;
}

////////////////////////////////////////////////////////////////////////

string FileSystemActionHandler::Action_OSVersion(FileSystem* pFS, const string& arguments)
{
  string result = "";

  return result;
}