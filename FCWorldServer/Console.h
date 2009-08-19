#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include <string>

class Player;

class Console
{
public:
  Console(Player& player);
  ~Console(void);

  bool executeCommand(std::string cmd, std::string curPath, std::string& result);
  ItemSoftware* getSoftwareForCommand(std::string cmd, bool bCaseSensitive);

private:

  void splitCommand(std::string& cmd, std::string& args);

  Player&               m_player;
};

#endif//_CONSOLE_H_