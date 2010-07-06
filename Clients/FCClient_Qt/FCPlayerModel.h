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

protected slots:
  
  void onLogin(QString username, QString password);
  void onCharacterSelected(FCUINT charID);
  void onInstallSoftware(short portNum, FCULONG itemID);
  void onUninstallSoftware(short portNum);

private:

  std::vector<Character*>       m_characters;
  QReadWriteLock                m_lockChars;
  Character*                    m_currentChar;

  ItemMgr                       m_itemMgr;
};

#endif // PLAYERMODEL_H
