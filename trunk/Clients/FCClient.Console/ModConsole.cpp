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
#include <stdio.h>
#include <iostream>
#include "../../common/protocol/fcprotocol.h"
#include "ModConsole.h"

ModConsole::ModConsole(void)
: m_pSink(NULL)
, m_characterID(0)
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
				m_state = WaitingForResponse;
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

      if ( !IsLocalCommand(cmd) )
      {
        m_pServer->SendConsoleCommand(m_currentDir, cmd);
        m_state = WaitingForResponse;
      }
      else
      {
        HandleLocalCommand(cmd);
      }

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
  m_currentDir = d.currentDir;

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

  m_currentDir = d->currentDir;

  printf("%s\n", d->result);

  delete [] (FCBYTE*)d;

  return true;
}

///////////////////////////////////////////////////////////////////////

bool ModConsole::IsLocalCommand(const char* cmd)
{
  char* localCmds[] =
  {
    "exit",
    "pwd",
    NULL
  };
  char** pCmd = localCmds;

  while ( *pCmd )
  {
    if ( !strcmp(*pCmd, cmd) )
      return true;
    pCmd++;
  }

  return false;
}

///////////////////////////////////////////////////////////////////////

void ModConsole::HandleLocalCommand(const char* cmd)
{
  string c = cmd;
  size_t pos = c.find(" ");

  if ( pos != string::npos )
    c = c.substr(0, pos);

  if ( c == "exit" )
  {
    if ( m_pSink )
      m_pSink->CloseModule(this);
  }
  else if ( c == "pwd" )
  {
    printf("%s\n", m_currentDir.c_str());
  }
}