#ifndef _SOFTWAREMGRWINDOW_H_
#define _SOFTWAREMGRWINDOW_H_

#include <string>
#include "../../common/game_objects/NetworkPorts.h"
#include "FCModel.h"
#include "FCDialog.h"
#include "IDesktop.h"

class SoftwareMgrWindow : public FCDialog
{
  DEFINE_IRRLICHT_FORM();

public:
  SoftwareMgrWindow(IDesktop* pDesktop, FCModel& model, IGUIEnvironment* pEnv, wchar_t* caption, IGUIElement* pParent, s32 id);
  ~SoftwareMgrWindow(void);

  bool OnButtonClicked(s32 id, IGUIButton* pBtn);

protected:
	static void OnOK(void* pCtx);
	static void OnCancel(void* pCtx);

private:

  /*
   *  Private Methods
   */
  void setLabels();
  void UpdateUIFromModel();
  void enablePort(FCSHORT port, bool bEnable);
  void setPortInfo(FCSHORT port, FCULONG itemID, FCULONG softwareType, bool bEnabled, u32 maxHealth, u32 health);

  /*
   *  Private Members
   */
  FCModel&                m_model;
  IrrlichtDevice*         m_pDevice;
  IDesktop*               m_pDesktop;

};

#endif//_SOFTWAREMGRWINDOW_H_