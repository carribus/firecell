#ifndef DLGLOGIN_H
#define DLGLOGIN_H

#include <QDialog>
#include "ui_DlgLogin.h"

class DlgLogin : public QDialog
{
  Q_OBJECT

public:
  DlgLogin(QWidget *parent);
  ~DlgLogin();

public slots:

  void accept();
  void done(int r);
  void reject();

  QString getUsername()                   { return m_username; }
  QString getPassword()                   { return m_password; }

private:
    
  Ui::DlgLogin    ui;
  QString         m_username;
  QString         m_password;
};

#endif // DLGLOGIN_H
