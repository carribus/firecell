#ifndef PLAYERMODEL_H
#define PLAYERMODEL_H

#include <QObject>
#include <QVector>
#include "dataobjects/Character.h"
#include "ItemMgr.h"

struct __FCPKT_CHARACTER_ITEMS_REQUEST_RESP;

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

  Item* addItems(__FCPKT_CHARACTER_ITEMS_REQUEST_RESP* d);
  ItemMgr& itemMgr()                              { return m_itemMgr; }

signals:

  void softwareInstallSucceeded(FCSHORT portNum, FCULONG itemID);
  void softwareInstallFailed(FCSHORT portNum, FCULONG itemID);
  void softwareUninstallSucceeded(FCSHORT portNum);
  void softwareUninstallFailed(FCSHORT portNum);
  void softwareApplicationAdded(FCULONG itemID);
  void networkPortStatusChangeSucceeded(bool bEnabled, FCSHORT portNum);
  void networkPortStatusChangeFailed(FCSHORT result, bool bEnabled, FCSHORT portNum);
  void softwareActivationSucceeded(FCULONG itemID);
  void softwareActivationFailed(FCULONG itemID, FCULONG result);

protected slots:
  
  void onLogin(QString username, QString password);
  void onCharacterSelected(FCUINT charID);
  void onInstallSoftware(FCSHORT portNum, FCULONG itemID);
  void onUninstallSoftware(FCSHORT portNum);
  void onEnableSoftwarePort(FCSHORT port, bool bEnable);
  void onSoftwareApplicationActivated(FCULONG itemID);

  // invokable methods
  Q_INVOKABLE void onSoftwareInstallResult(bool bResult, FCSHORT portNum, FCULONG itemID);
  Q_INVOKABLE void onSoftwareUninstallResult(bool bResult, FCSHORT portNum);
  Q_INVOKABLE void onNetworkPortStatusChangeResult(FCSHORT result, bool bEnabled, FCSHORT portNum);
  Q_INVOKABLE void onActivateSoftwareResult(FCULONG result, FCULONG itemID, FCSHORT cpuCost, FCULONG memCost);

private:

  std::vector<Character*>       m_characters;
  QReadWriteLock                m_lockChars;
  Character*                    m_currentChar;

  ItemMgr                       m_itemMgr;
};

#endif // PLAYERMODEL_H
