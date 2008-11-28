#ifndef _FILESYSTEMACTIONHANDLER_H_
#define _FILESYSTEMACTIONHANDLER_H_

#include <string>
#include "FileSystem.h"

using namespace std;

class FileSystemActionHandler
{
public:
  FileSystemActionHandler(void);
  ~FileSystemActionHandler(void);

  //
  // Action methods
  static string Action_FileListing(FileSystem* pFS, const string& arguments);
  static string Action_ChangeDirectory(FileSystem* pFS, const string& arguments);
  static string Action_OSVersion(FileSystem* pFS, const string& arguments);

};

#endif//_FILESYSTEMACTIONHANDLER_H_