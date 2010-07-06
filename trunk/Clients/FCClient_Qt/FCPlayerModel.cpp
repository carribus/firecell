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

void FCPlayerModel::onInstallSoftware(short portNum, FCULONG itemID)
{
  FCPlayerModel* player = FCAPP->playerModel();
  ItemMgr& itemMgr = player->itemMgr();
  Item* pItem = NULL;
  ItemMgr::GameItem item;

  itemMgr.getItem( itemID, item );
  qDebug() << item.getItem()->GetName().c_str();
}

///////////////////////////////////////////////////////////////////////

void FCPlayerModel::onUninstallSoftware(short portNum)
{
  qDebug() << "Uninstall software on port " << portNum;
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
