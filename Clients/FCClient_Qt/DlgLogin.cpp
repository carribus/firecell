#include "StdAfx.h"
#include "DlgLogin.h"

DlgLogin::DlgLogin(QWidget *parent)
: QDialog(parent)
{
  ui.setupUi(this);
}

///////////////////////////////////////////////////////////////////////

DlgLogin::~DlgLogin()
{
}

///////////////////////////////////////////////////////////////////////

void DlgLogin::accept()
{
  m_username = ui.edtUsername->text();
  m_password = ui.edtPassword->text();
  QDialog::accept();
}

///////////////////////////////////////////////////////////////////////

void DlgLogin::done(int r)
{
  QDialog::done(r);
}

///////////////////////////////////////////////////////////////////////

void DlgLogin::reject()
{
  QDialog::reject();
}
