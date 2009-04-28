#ifndef _SOFTWAREMGRWINDOW_H_
#define _SOFTWAREMGRWINDOW_H_

#include <string>
#include "../../common/game_objects/NetworkPorts.h"
#include "FCModel.h"
#include "FCDialog.h"
#include "IDesktop.h"

class FCController;

class SoftwareMgrWindow : public FCDialog
{
  DEFINE_IRRLICHT_FORM();

public:
  SoftwareMgrWindow(IDesktop* pDesktop, FCModel& model, IGUIEnvironment* pEnv, wchar_t* caption, IGUIElement* pParent, s32 id);
  ~SoftwareMgrWindow(void);

  void Update();

  bool OnButtonClicked(s32 id, IGUIButton* pBtn);
  bool OnMenuItemSelected(s32 selectedItem, IGUIContextMenu* pMenu);
  bool OnUnknownGUIEvent(u32 eventType, s32 id, IGUIElement* pElem);

  void setController(FCController* pController)                         { m_pController = pController; }

protected:
	static void OnOK(void* pCtx);
	static void OnCancel(void* pCtx);

private:

  /*
   *  Private Methods
   */
  void setLabels();
  void UpdateUIFromModel();
//  void PopulateComboBoxes();
  void enablePort(FCSHORT port, bool bEnable);
  void setPortInfo(FCSHORT port, FCULONG itemID, FCULONG softwareType, bool bEnabled, u32 maxHealth, u32 health);
  void popupPortMenu(FCSHORT portNum);

  /*
   *  Private Members
   */
  FCModel&                m_model;
  IrrlichtDevice*         m_pDevice;
  IDesktop*               m_pDesktop;
  FCController*           m_pController;

  struct stMenuItem
  {
    u32 menuIndex;
    FCSHORT targetPort;
    FCULONG itemID;
  };
  typedef std::vector<stMenuItem> MenuItemVector;
  MenuItemVector          m_menuItems;

};

#endif//_SOFTWAREMGRWINDOW_H_