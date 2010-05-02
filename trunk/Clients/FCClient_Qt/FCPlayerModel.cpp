#ifdef _USE_STDAFX_H_
  #include "StdAfx.h"
#endif//_USE_STDAFX_H_
#include "FCPlayerModel.h"
#include "FCApp.h"

FCPlayerModel::FCPlayerModel(QObject *parent)
: QObject(parent)
, m_lockChars(QReadWriteLock::Recursive)
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

int FCPlayerModel::addCharacter(Character& character)
{
  QReadLocker lock(&m_lockChars);

  m_characters.push_back(character);

  return m_characters.size();
}