#ifndef _DLGSOFTWAREMGR_H_
#define _DLGSOFTWAREMGR_H_

#include "../../common/fctypes.h"
#include <QDialog>
#include "ui_DlgSoftwareMgr.h"

class DlgSoftwareMgr : public QDialog
{
  Q_OBJECT

public:
  DlgSoftwareMgr(QWidget* parent);
  ~DlgSoftwareMgr(void);

signals:
  void installSoftware(short portNum, FCULONG itemID);
  void uninstallSoftware(short portNum);

protected slots:

  void onLinkActivated(const QString& link);
  void updateUIFromModel();
  
private:

  void enablePort(FCSHORT port, bool bEnable);
  void setPortInfo(FCSHORT port, FCULONG itemID, FCULONG softwareType, bool bEnabled, quint32 maxHealth, quint32 health);

  Ui::DlgSoftwareMgr    ui;

};

#endif//_DLGSOFTWAREMGR_H_