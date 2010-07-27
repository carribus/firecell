/*
    FireCell Qt Client - The Qt client code for the firecell multiplayer game
    Copyright (C) 2008-2010  Peter M. Mares

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
#ifdef _USE_STDAFX_H_
  #include "StdAfx.h"
#endif//_USE_STDAFX_H_
#include "FCPlayerModel.h"
#include "FCApp.h"

FCPlayerModel::FCPlayerModel(QObject *parent)
: QObject(parent)
, m_lockChars(QReadWriteLock::Recursive)
, m_currentChar(NULL)
, m_itemMgr(this)
{

}

///////////////////////////////////////////////////////////////////////

FCPlayerModel::~FCPlayerModel()
{

}

///////////////////////////////////////////////////////////////////////

void FCPlayerModel::onLogin(QString username, QString password)
{
  FCAPP->network().sendLogin(username, password);
}

///////////////////////////////////////////////////////////////////////

void FCPlayerModel::onCharacterSelected(FCUINT charID)
{
  FCAPP->network().sendCharacterSelection(charID);
}

///////////////////////////////////////////////////////////////////////

void FCPlayerModel::onInstallSoftware(FCSHORT portNum, FCULONG itemID)
{
  FCAPP->network().sendInstallSoftwareRequest(portNum, itemID);
}

///////////////////////////////////////////////////////////////////////

void FCPlayerModel::onUninstallSoftware(FCSHORT portNum)
{
  FCAPP->network().sendUninstallSoftwareRequest(portNum);
}

///////////////////////////////////////////////////////////////////////

void FCPlayerModel::onEnableSoftwarePort(FCSHORT port, bool bEnable)
{
  FCAPP->network().sendNetworkPortEnableRequest(port, bEnable);
}

///////////////////////////////////////////////////////////////////////

void FCPlayerModel::onSoftwareInstallResult(bool bResult, FCSHORT portNum, FCULONG itemID)
{
  ItemMgr::GameItem item;

  if ( bResult )
  {
    if ( m_itemMgr.getItem(itemID, item) )
    {
      ItemSoftware* pItem = (ItemSoftware*)item.getItem();

      if ( pItem )
      {
        NetworkPorts& ports = m_currentChar->GetComputer().GetNetworkPorts();

        m_itemMgr.setItemCount( itemID, item.getCount()-1 );
        ports.installPort(portNum, pItem->GetSoftwareType(), itemID);

        emit softwareInstallSucceeded(portNum, itemID);
      }
    }
  }
  else
  {
    emit softwareInstallFailed(portNum, itemID);
  }
}

///////////////////////////////////////////////////////////////////////

void FCPlayerModel::onSoftwareUninstallResult(bool bResult, FCSHORT portNum)
{
  ItemMgr::GameItem item;
  FCULONG itemID, softwareType;
  NetworkPorts& ports = m_currentChar->GetComputer().GetNetworkPorts();

  if ( bResult )
  {
    ports.getSoftwareInfo(portNum, itemID, softwareType);
    ports.uninstallPort(portNum);
    if ( m_itemMgr.getItem(itemID, item) )
    {
      m_itemMgr.setItemCount( itemID, item.getCount()+1 );
    }
    else
    {
      // the item doesn't exist in the item manager
    }
    
    emit softwareUninstallSucceeded(portNum);
  }
  else
  {
    emit softwareUninstallFailed(portNum);
  }
}

///////////////////////////////////////////////////////////////////////

void FCPlayerModel::onNetworkPortStatusChangeResult(FCSHORT result, bool bEnabled, FCSHORT portNum)
{
  NetworkPorts& ports = m_currentChar->GetComputer().GetNetworkPorts();

  ports.lockForWrite();
  // updated the enabled status of the port
  ports.enablePort( portNum, bEnabled );

  switch ( result )
  {
  case  NPE_OK:
    emit networkPortStatusChangeSucceeded(bEnabled, portNum);
    break;

  case  NPE_NO_SERVICE_ASSIGNED:
  case  NPE_NO_SOFTWARETYPE:
  case  NPE_PORT_DOESNT_EXIST:
    emit networkPortStatusChangeFailed(result, bEnabled, portNum);
    break;
  
  default:
    break;
  }
  ports.unlock();
}

///////////////////////////////////////////////////////////////////////

int FCPlayerModel::addCharacter(Character* character)
{
  QReadLocker lock(&m_lockChars);

  m_characters.push_back(character);

  return m_characters.size();
}

///////////////////////////////////////////////////////////////////////

Character* FCPlayerModel::getCharacter(int nIndex)
{
  QReadLocker lock(&m_lockChars);
  Character* pChar = NULL;

  if ( nIndex >= 0 && nIndex < (int)m_characters.size() )
  {
    pChar = m_characters[nIndex];
  }

  return pChar;
}

///////////////////////////////////////////////////////////////////////

bool FCPlayerModel::selectCharacter(FCUINT characterID)
{
  QReadLocker lock(&m_lockChars);
  std::vector<Character*>::iterator it = m_characters.begin();
  std::vector<Character*>::iterator limit = m_characters.end();

  for ( ; it != limit; ++it )
  {
    if ( (*it)->GetID() == characterID )
    {
      m_currentChar = *it;
      break;
    }
  }

  return (m_currentChar != NULL);
}
