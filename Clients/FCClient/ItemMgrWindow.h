#ifndef _ITEMMGRWINDOW_H_
#define _ITEMMGRWINDOW_H_

#include <string>
#include "FCModel.h"
#include "FCDialog.h"
#include "IDesktop.h"

class FCController;

class ItemMgrWindow : public FCDialog
{
  DEFINE_IRRLICHT_FORM();

public:
  ItemMgrWindow(IDesktop* pDesktop, FCModel& model, IGUIEnvironment* pEnv, wchar_t* caption, IGUIElement* pParent, s32 id);
  ~ItemMgrWindow(void);

  void setController(FCController* pController)                         { m_pController = pController; }

protected:
	static void OnOK(void* pCtx);
	static void OnCancel(void* pCtx);

  void UpdateUIFromModel();

private:
  /*
   *  Private Members
   */
  FCModel&                m_model;
  IrrlichtDevice*         m_pDevice;
  IDesktop*               m_pDesktop;
  FCController*           m_pController;

};

#endif//_ITEMMGRWINDOW_H_