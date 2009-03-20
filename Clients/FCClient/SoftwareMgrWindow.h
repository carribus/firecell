#ifndef _SOFTWAREMGRWINDOW_H_
#define _SOFTWAREMGRWINDOW_H_

#include <string>
#include "FCModel.h"
#include "FCDialog.h"
#include "IDesktop.h"

class SoftwareMgrWindow : public FCDialog
{
  DEFINE_IRRLICHT_FORM();

public:
  SoftwareMgrWindow(IDesktop* pDesktop, IGUIEnvironment* pEnv, wchar_t* caption, IGUIElement* pParent, s32 id);
  ~SoftwareMgrWindow(void);

protected:
	static void OnOK(void* pCtx);
	static void OnCancel(void* pCtx);

private:

  /*
   *  Private Methods
   */
  void setLabels();

  /*
   *  Private Members
   */
  IrrlichtDevice*         m_pDevice;
  IDesktop*               m_pDesktop;

};

#endif//_SOFTWAREMGRWINDOW_H_