#include <sstream>
#include "Country.h"
#include "City.h"
#include "DlgNewCharacter.h"

DECLARE_FORM_ELEMENTS(DlgNewCharacter)
  FORM_ELEMENT("staticText"		  , 1		          , 10		, 40		, 150			, 25			, L"Character Name : ")
  FORM_ELEMENT("editBox"				, 2		          , 155		, 40		, 250			, 25			, NULL)
	FORM_ELEMENT("staticText"		  , 3		          , 10		, 80		, 150			, 25			, L"Country : ")
	FORM_ELEMENT("comboBox"			  , 4		          , 155		, 80		, 250			, 25			, NULL)
	FORM_ELEMENT("staticText"		  , 5		          , 10		, 120		, 150			, 25			, L"City : ")
	FORM_ELEMENT("comboBox"			  , 6		          , 155		, 120		, 250			, 25			, NULL)
  FORM_ELEMENT("button"         , FCBTN_CANCEL  , 170   , 160   , 110     , 25     , L"Cancel")
  FORM_ELEMENT("button"         , FCBTN_OK      , 290   , 160   , 110     , 25     , L"Create")
END_FORM_ELEMENTS()

IMPLEMENT_IRRLICHT_FORM(DlgNewCharacter);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DlgNewCharacter::DlgNewCharacter(IGUIEnvironment* pEnv, wchar_t* caption, IGUIElement* pParent, s32 id)
: FCDialog(pEnv, pParent, caption, true, id, rect<s32>(0, 0, 450, 200))
{
	// create the controls of the object
  createFormElements();
  Environment->setFocus( getElementFromId(2) );
  CenterWindow();
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
	u32 itemIndex = 0;
  IGUIComboBox* pCountryCB = (IGUIComboBox*) getElementFromId(4, true);

  if ( pCountryCB )
  {
    for ( ; it != limit; it++ )
    {
      ss.str(L"");
      ss << it->second.GetName().c_str();
      itemIndex = pCountryCB->addItem( ss.str().c_str() );
			m_mapCountryIndexToID[itemIndex] = it->second.GetID();
    }
  }

	m_mapCountries = countries;

	// get the currently selected item in the countries combo box
	s32 selItem = pCountryCB->getSelected();
	if ( selItem != -1 )
	{
		// get the matching countryID for the selected item
		FCULONG countryID = m_mapCountryIndexToID[selItem];
		populateCitiesComboBox(countryID);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DlgNewCharacter::populateCitiesComboBox(FCULONG countryID)
{
	std::map<FCULONG, Country>::iterator it = m_mapCountries.find(countryID);

	if ( it != m_mapCountries.end() )
	{
	  std::wstringstream ss;
		std::map<FCULONG, City>& cities = it->second.GetCities();
		std::map<FCULONG, City>::iterator cityIT = cities.begin();
		IGUIComboBox* pCityCB = (IGUIComboBox*) getElementFromId(6, true);

    pCityCB->clear();

		while ( cityIT != cities.end() )
		{
      ss.str(L"");
      ss << cityIT->second.GetName().c_str();
			pCityCB->addItem( ss.str().c_str() );
			cityIT++;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool DlgNewCharacter::OnComboBoxChanged(s32 id, IGUIComboBox* pCB)
{
	switch ( id )
	{
	case	4:				// country combo
    {
      s32 selItem = pCB->getSelected();
      FCULONG countryID = m_mapCountryIndexToID[selItem];

      populateCitiesComboBox(countryID);
    }
		break;

	case	6:				// city combo
    {
    }
		break;

	default:
		break;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool DlgNewCharacter::OnButtonClicked(s32 id, IGUIButton* pBtn)
{
  switch ( id )
  {
  case  FCBTN_OK:
    OnOK();
    break;

  case  FCBTN_CANCEL:
    OnCancel();
    break;
  }

  return true;
}