#ifndef _FCCONSTANTS_H_
#define _FCCONSTANTS_H_

/**
 *  \brief  This enumeration specified the different service roles in the FireCell distributed network
 */
enum ServiceType
{
  ST_None,
  ST_Router,
  ST_Auth,
  ST_World,
  ST_Community,
  ST_Client       // this should always be the last in the enum
};

/**
 *  \brief  Status result of a login attempt
 */
enum e_LoginStatus
{
  LoginFailed = 0,
  LoginSuccess = 1,
  LoginAccountLoggedInAlready = 2
};
/**
 *  \brief Desktop Option types available to the player in the client
 */
enum DesktopOptionType
{
  DOT_Forum = 1,
  DOT_News,
  DOT_Email,
  DOT_Console,
  DOT_Bank,
  DOT_Chat,
  DOT_HackingTools
};


/**
 *  \brief Application activation results
 */
#define ACTIVATERESULT_UNKNOWN_APP      0x00
#define ACTIVATERESULT_OK               0x01
#define ACTIVATERESULT_NOT_ENOUGH_MEM   0x02
#define ACTIVATERESULT_NOT_ENOUGH_CPU   0x04
#define ACTIVATERESULT_NEED_HTTP        0x08
#define ACTIVATERESULT_NEED_FTP         0x10
#define ACTIVATERESULT_NEED_SSH         0x20
#define ACTIVATERESULT_NEED_BANK        0x40
#define ACTIVATERESULT_NEED_DB          0x80
#define ACTIVATERESULT_NEED_MAIL        0x100

/**
 *  \brief  Status of software installation attempt
 */
/*
enum e_SoftwareInstallResult
{
  SIR_Success = 0,
  SIR_Failed_SoftwareAlreadyAssignToPort,
  SIR_Failed_Memory,
  SIR_Failed_CPU,
  SIR_Failed_PortHealth
};
*/
#endif//_FCCONSTANTS_H_