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

  if ( nIndex >= 0 && nIndex < m_characters.size() )
  {
    pChar = m_characters[nIndex];
  }

  return pChar;
}

///////////////////////////////////////////////////////////////////////

bool FCPlayerModel::selectCharacter(FCUINT characterID)
{
  QReadLocker lock(&m_lockChars);
  Character* pChar = NULL;
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