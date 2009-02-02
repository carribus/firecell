#ifndef _DLGNEWCHARACTER_H_
#define _DLGNEWCHARACTER_H_

#include <map>
#include <string>
#include "../../common/fctypes.h"
#include "FCDialog.h"
#include "Country.h"

class DlgNewCharacter :	public FCDialog
{
public:
	DlgNewCharacter(IGUIEnvironment* pEnv, wchar_t* caption = L"Create a new character", IGUIElement* pParent = 0, s32 id = -1);
	~DlgNewCharacter(void);

  void populateGeographyControls(std::map<FCULONG, Country>& countries);

private:

  DEFINE_IRRLICHT_FORM();

  /*
   *  Private data members
   */

  // Form elements
  std::string                  m_strCharName;
};

#endif//_DLGNEWCHARACTER_H_