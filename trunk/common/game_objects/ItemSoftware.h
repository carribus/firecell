#ifndef _ITEMSOFTWARE_H_
#define _ITEMSOFTWARE_H_

#include "Item.h"
#ifndef _FCCLIENT_COMPILE_
#include "../interfaces/ISoftwareLogic.h"
class Player;
#endif

class ItemSoftware : public Item
{
public:
  ItemSoftware(void);
  ~ItemSoftware(void);

#ifndef _FCCLIENT_COMPILE_
  bool Execute(Player* pPlayer, const std::string& cmd, const std::string& args, std::string& result);
#endif//

  void SetSoftwareType(FCSHORT type)                        { m_swType = type; }
  void IsService(bool bFlag)                                { m_bIsService = bFlag; }
  void SetCPUCost(FCSHORT cost)                             { m_cpuCost = cost; }
  void SetMemCost(FCULONG cost)                             { m_memCost = cost; }
  void SetDesktopIconFlag(bool bFlag)                       { m_bDesktopFlag = bFlag; }
#ifndef _FCCLIENT_COMPILE_
  void SetCommand(std::string cmd)                          { m_command = cmd; }
  void SetLogic(ISoftwareLogic* pLogic)                     { m_pLogic = pLogic; }
#endif//

  FCSHORT GetSoftwareType()                                 { return m_swType; }
  bool IsService()                                          { return m_bIsService; }
  FCSHORT GetCPUCost()                                      { return m_cpuCost; }
  FCULONG GetMemCost()                                      { return m_memCost; }
  bool GetDesktopIconFlag()                                 { return m_bDesktopFlag; }
#ifndef _FCCLIENT_COMPILE_
  std::string GetCommand()                                  { return m_command; }
#endif

protected:

  FCSHORT m_swType;                 //!> Software Type
  bool m_bIsService;                //!> Flag indicating whether software is a service or a tool
  FCULONG m_scriptID;               //!> Temporary ID holder until we have a script object
  FCSHORT m_cpuCost;                //!> CPU Cost of the software
  FCULONG m_memCost;                //!> Memory cost of the software
  bool m_bDesktopFlag;              //!> Flag indicating whether the client should show this item on the desktop
#ifndef _FCCLIENT_COMPILE_
  std::string m_command;            //!> The command that this software will respond to from a command line
  ISoftwareLogic* m_pLogic;         //!> Additional logic that may be attached to this item
#endif//
};

#endif//_ITEMSOFTWARE_H_