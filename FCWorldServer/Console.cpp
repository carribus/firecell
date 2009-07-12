#include "Computer.h"
#include "FileSystem.h"
#include "ItemManager.h"
#include "Player.h"
#include "Console.h"

Console::Console(Player& player)
: m_player(player)
{
}

///////////////////////////////////////////////////////////////////////

Console::~Console(void)
{
}

///////////////////////////////////////////////////////////////////////

bool Console::executeCommand(std::string cmd, std::string curPath, std::string& result)
{
  bool bResult = false;
  ItemManager& itemMgr = ItemManager::instance();
  FileSystem& fs = m_player.GetComputer().GetFileSystem();
  std::string args, strRes;

  // split the command up into command and arguments
  splitCommand(cmd, args);

  if ( fs.CanHandleCommand(cmd) )
  {
    bResult = fs.ExecuteCommand( &m_player, cmd, args, result );
  }
  else
  {
    Item* pItem = NULL;
    ItemSoftware* pSoftware = NULL;
    // check if we can execute one of the software items
    std::map<FCULONG, Player::PlayerItem> items = m_player.GetItems();
    std::map<FCULONG, Player::PlayerItem>::iterator it = items.begin();
    std::map<FCULONG, Player::PlayerItem>::iterator limit = items.end();

    for ( ; it != limit; ++it )
    {
      pItem = NULL;
      pSoftware = NULL;
      if ( it->second.count )
      {
        pItem = itemMgr.GetItem( it->second.itemID );
      }
      if ( pItem && pItem->GetTypeID() == ItemType::Software )
      {
        if ( (pSoftware = static_cast<ItemSoftware*>(pItem)) )
        {
          std::string itemCmd = pSoftware->GetCommand();
          bool bIsCommand = false;

          if ( fs.IsCaseSensitive() )
          {
            bIsCommand = ( itemCmd.compare( cmd ) == 0 );
          }
          else
          {
            std::string c1 = cmd, c2 = itemCmd;
            std::transform(c1.begin(), c1.end(), c1.begin(), (int(*)(int))tolower);
            std::transform(c2.begin(), c2.end(), c2.begin(), (int(*)(int))tolower);
            bIsCommand = ( c1.compare(c2) == 0 );
          }
            
          if ( bIsCommand )
          {
            cmd = pSoftware->GetCommand();
            bResult = pSoftware->Execute(cmd, args, result);
          }
        }
      }
    }
  }

  if ( !bResult )
  {
    result = "Unrecognised command\n\n";
  }

  return bResult;
}

///////////////////////////////////////////////////////////////////////

void Console::splitCommand(std::string& cmd, std::string& args)
{
  size_t pos;

  if ( cmd.length() > 0 )
  {
    pos = cmd.find_first_of(" ");
    if ( pos != string::npos )
    {
      args = cmd.substr(cmd.find_first_of(" ")+1, cmd.length());
      cmd.erase(cmd.find_first_of(" "), cmd.length());
    }
  }
}