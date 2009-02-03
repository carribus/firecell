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

protected:

	bool OnComboBoxChanged(s32 id, IGUIComboBox* pCB);

private:

	void populateCitiesComboBox(FCULONG countryID);

  DEFINE_IRRLICHT_FORM();

  /*
   *  Private data members
   */

  // Form elements
  std::string                 m_strCharName;

	// copy of the country's map from the model
	std::map<FCULONG, Country>	m_mapCountries;
	// country combo index to object mapping
	std::map<u32, FCULONG>			m_mapCountryIndexToID;
};

#endif//_DLGNEWCHARACTER_H_