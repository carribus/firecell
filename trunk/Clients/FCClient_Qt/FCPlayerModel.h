#ifndef PLAYERMODEL_H
#define PLAYERMODEL_H

#include <QObject>
#include <QVector>
#include "dataobjects/Character.h"

class FCPlayerModel : public QObject
{
  Q_OBJECT

public:
  FCPlayerModel(QObject *parent);
  ~FCPlayerModel();

  int addCharacter(Character& character);

protected slots:
  
  void onLogin(QString username, QString password);

private:

  std::vector<Character>        m_characters;
  QReadWriteLock                m_lockChars;
};

#endif // PLAYERMODEL_H
