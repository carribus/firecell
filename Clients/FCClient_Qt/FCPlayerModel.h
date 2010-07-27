#ifndef PLAYERMODEL_H
#define PLAYERMODEL_H

#include <QObject>
#include <QVector>
#include "dataobjects/Character.h"
#include "ItemMgr.h"

class FCPlayerModel : public QObject
{
  Q_OBJECT

public:
  FCPlayerModel(QObject *parent);
  ~FCPlayerModel();

  int addCharacter(Character* character);
  Character* getCharacter(int nIndex);
  bool selectCharacter(FCUINT characterID);
  int characterCount()                            { return m_characters.size(); }
  Character* getCurrentCharacter()                { return m_currentChar; }

  Item* addItem();
  ItemMgr& itemMgr()                              { return m_itemMgr; }

signals:

  void softwareInstallSucceeded(FCSHORT portNum, FCULONG itemID);
  void softwareInstallFailed(FCSHORT portNum, FCULONG itemID);
  void softwareUninstallSucceeded(FCSHORT portNum);
  void softwareUninstallFailed(FCSHORT portNum);
  void networkPortStatusChangeSucceeded(bool bEnabled, FCSHORT portNum);
  void networkPortStatusChangeFailed(FCSHORT result, bool bEnabled, FCSHORT portNum);

protected slots:
  
  void onLogin(QString username, QString password);
  void onCharacterSelected(FCUINT charID);
  void onInstallSoftware(FCSHORT portNum, FCULONG itemID);
  void onUninstallSoftware(FCSHORT portNum);
  void onEnableSoftwarePort(FCSHORT port, bool bEnable);

  // invokable methods
  Q_INVOKABLE void onSoftwareInstallResult(bool bResult, FCSHORT portNum, FCULONG itemID);
  Q_INVOKABLE void onSoftwareUninstallResult(bool bResult, FCSHORT portNum);
  Q_INVOKABLE void onNetworkPortStatusChangeResult(FCSHORT result, bool bEnabled, FCSHORT portNum);

private:

  std::vector<Character*>       m_characters;
  QReadWriteLock                m_lockChars;
  Character*                    m_currentChar;

  ItemMgr                       m_itemMgr;
};

#endif // PLAYERMODEL_H
