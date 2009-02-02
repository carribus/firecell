#include <sstream>
#include "Country.h"
#include "City.h"
#include "DlgNewCharacter.h"

DECLARE_FORM_ELEMENTS(DlgNewCharacter)
  FORM_ELEMENT("staticText"		  , 1		, 10		, 50		, 150			, 25			, L"Character Name : ")
  FORM_ELEMENT("editBox"				, 2		, 155		, 50		, 250			, 25			, NULL)
	FORM_ELEMENT("staticText"		  , 3		, 10		, 90		, 150			, 25			, L"Country : ")
	FORM_ELEMENT("comboBox"			  , 4		, 155		, 90		, 250			, 25			, NULL)
	FORM_ELEMENT("staticText"		  , 5		, 10		, 130		, 150			, 25			, L"City : ")
	FORM_ELEMENT("comboBox"			  , 6		, 155		, 130		, 250			, 25			, NULL)
END_FORM_ELEMENTS()

IMPLEMENT_IRRLICHT_FORM(DlgNewCharacter);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DlgNewCharacter::DlgNewCharacter(IGUIEnvironment* pEnv, wchar_t* caption, IGUIElement* pParent, s32 id)
: FCDialog(pEnv, pParent, caption, true, id)
{
	// create the controls of the object
  createFormElements();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DlgNewCharacter::~DlgNewCharacter(void)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DlgNewCharacter::populateGeographyControls(std::map<FCULONG, Country>& countries)
{
  if ( !countries.size() )
    return;

  std::wstringstream ss;
  std::map<FCULONG, Country>::iterator it = countries.begin();
  std::map<FCULONG, Country>::iterator limit = countries.end();
  IGUIComboBox* pCountryCB = (IGUIComboBox*) getElementFromId(4, true);

  if ( pCountryCB )
  {
    for ( ; it != limit; it++ )
    {
      ss.str(L"");
      ss << it->second.GetName().c_str();
      u32 itemIndex = pCountryCB->addItem( ss.str().c_str() );
    }
  }
}