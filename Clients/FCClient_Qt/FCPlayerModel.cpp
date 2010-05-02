#include "StdAfx.h"
#include "FCPlayerModel.h"
#include "FCApp.h"

FCPlayerModel::FCPlayerModel(QObject *parent)
: QObject(parent)
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