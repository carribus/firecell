#include <stdio.h>
#include <iostream>
#include "../../common/protocol/fcprotocol.h"
#include "ModConsole.h"

ModConsole::ModConsole(void)
: m_characterID(0)
, m_pServer(NULL)
, m_state(ModConsole::Initialise)
, m_currentDir("/")
{
}

///////////////////////////////////////////////////////////////////////

ModConsole::~ModConsole(void)
{
}

///////////////////////////////////////////////////////////////////////

void ModConsole::QueueForAction()
{
  switch ( m_state )
  {
  case  ModConsole::Initialise:
    {
      if ( m_pServer )
      {
        m_pServer->RequestCharacterFileSystemInfo(m_characterID);
//        m_pServer->RequestFileSystemList(m_currentDir);
      }
    }
    break;

  case  ModConsole::Running:
    {
      char cmd[256];
      memset(cmd, 0, sizeof(cmd));
      printf("%s: ", m_currentDir.c_str());
      cin.getline(cmd, sizeof(cmd)-1);

      m_pServer->SendConsoleCommand(m_currentDir, cmd);

      m_state = WaitingForResponse;
    }
    break;

  case  ModConsole::WaitingForResponse:
    break;

  default:
    printf("[Console] Unknown state reached (%ld)\n", m_state);
    break;
  }
}

///////////////////////////////////////////////////////////////////////

bool ModConsole::OnResponse(FCSHORT msgID, PEPacket* pPkt, BaseSocket* pSocket)
{
  bool bHandled = false;

  switch ( msgID )
  {
  case  FCMSG_CON_GET_FS_INFO:
    bHandled = OnResponseConGetFSInfo(pPkt, pSocket);
    break;

  case  FCMSG_CON_GET_FILE_LIST:
    bHandled = OnResponseConGetFileList(pPkt, pSocket);
    break;

  case  FCMSG_CON_COMMAND:
    bHandled = OnResponseConCommand(pPkt, pSocket);

  default:
    break;
  }

  if ( bHandled )
    m_state = ModConsole::Running;

  return bHandled;
}

///////////////////////////////////////////////////////////////////////

bool ModConsole::OnResponseConGetFSInfo(PEPacket* pPkt, BaseSocket* pSocket)
{
  __FCPKT_CON_GET_FS_INFO_RESP d;
  size_t dataLen;

  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  pPkt->GetField("data", &d, dataLen);

  m_dirSeperator = d.dirSeperator;
  m_fsStyle = (FCBYTE) d.fsStyle & 0x000000FF;

  m_state = Running;

  return true;
}

///////////////////////////////////////////////////////////////////////

bool ModConsole::OnResponseConGetFileList(PEPacket* pPkt, BaseSocket* pSocket)
{
  __FCPKT_CON_GET_FILE_LIST_RESP* d;
  size_t dataLen;

  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  d = (__FCPKT_CON_GET_FILE_LIST_RESP*) new FCBYTE[ dataLen ];
  pPkt->GetField("data", d, dataLen);

  printf("File listing for: %s\n", m_currentDir.c_str());
  for ( FCULONG i = 0; i < d->numFiles; i++ )
  {
    // get the files and then show them
    printf("%-32s %-12s %-12s\n", d->Files[i].filename, d->Files[i].is_dir ? "<Dir>" : "", d->Files[i].is_mutable ? "--r--r--r" : "wrxwrxwrx");
  }
  printf("\n%ld Files\n\n", d->numFiles);

  delete [] (FCBYTE*)d;

  return true;
}

///////////////////////////////////////////////////////////////////////

bool ModConsole::OnResponseConCommand(PEPacket* pPkt, BaseSocket* pSocket)
{
  __FCPKT_CON_COMMAND_RESP* d;
  size_t dataLen;

  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  d = (__FCPKT_CON_COMMAND_RESP*) new FCBYTE[ dataLen ];
  pPkt->GetField("data", d, dataLen);

  printf(d->result);

  delete [] (FCBYTE*)d;

  return true;
}