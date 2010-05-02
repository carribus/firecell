#ifndef PLAYERMODEL_H
#define PLAYERMODEL_H

#include "StdAfx.h"
#include <QObject>

class FCPlayerModel : public QObject
{
  Q_OBJECT

public:
  FCPlayerModel(QObject *parent);
  ~FCPlayerModel();

protected slots:
  
  void onLogin(QString username, QString password);

private:
};

#endif // PLAYERMODEL_H
