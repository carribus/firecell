#ifndef _DLGNEWCHARACTER_H_
#define _DLGNEWCHARACTER_H_

#include "FCDialog.h"

class DlgNewCharacter :	public FCDialog
{
public:
	DlgNewCharacter(IGUIEnvironment* pEnv, wchar_t* caption = L"Create a new character", IGUIElement* pParent = 0, s32 id = -1);
	~DlgNewCharacter(void);

private:

	struct FormElement
	{
		char*								elemType;
		s32									id;
		s32									xPos,
												yPos,
												width,
												height;
		wchar_t*						text;
	};
	static FormElement		m_arrFormElements[];

	IGUIElement* createFormElement(FormElement& fe);
};

#endif//_DLGNEWCHARACTER_H_