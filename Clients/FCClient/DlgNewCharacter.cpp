#include "DlgNewCharacter.h"

DlgNewCharacter::FormElement DlgNewCharacter::m_arrFormElements[] =
{
	/* elem type name	, id	, xpos	, ypos	, width		, height	, text */
	{ "staticText"		, 1		, 10		, 50		, 150			, 25			, L"Character Name : " },
	{ "editBox"				, 2		, 155		, 50		, 250			, 25			, NULL },
	{ "staticText"		, 3		, 10		, 90		, 150			, 25			, L"Location : " },
	{ "comboBox"			, 4		, 155		, 90		, 250			, 25			, NULL },
	{ NULL						, -1	, 0			, 0			, 0				, 0				, NULL }
};

DlgNewCharacter::DlgNewCharacter(IGUIEnvironment* pEnv, wchar_t* caption, IGUIElement* pParent, s32 id)
: FCDialog(pEnv, pParent, caption, true, id)
{
	// create the controls of the object
	FormElement* pFE = m_arrFormElements;
	while ( pFE->elemType )
	{
		createFormElement(*pFE);
		pFE++;
	}
}

DlgNewCharacter::~DlgNewCharacter(void)
{
}

IGUIElement* DlgNewCharacter::createFormElement(FormElement& fe)
{
	IGUIElement* pElem = Environment->addGUIElement( fe.elemType, this );

	if ( pElem )
	{
		pElem->setID( fe.id );
		pElem->setRelativePosition( core::rect<s32>( fe.xPos, fe.yPos, fe.xPos + fe.width, fe.yPos + fe.height ) );
		pElem->setText( fe.text );
	}

	return pElem;
}