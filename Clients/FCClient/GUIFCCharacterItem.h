#ifndef _GUIFCCHARACTERITEM_H_
#define _GUIFCCHARACTERITEM_H_

#include <irrlicht.h>

using namespace irr;
using namespace gui;

class GUIFCCharacterItem : public IGUIElement
{
public:
	GUIFCCharacterItem(IGUIEnvironment* pEnv, IGUIElement* pParent = NULL, s32 id = -1);
	~GUIFCCharacterItem(void);

	/*
	 *	IGUIElement overrides
	 */
	void draw();

	/*
	 *	Public Methods
	 */
	void SetPosition(s32 left, s32 top);

private:

	core::position2d<s32>						m_position;
};

#endif//_GUIFCCHARACTERITEM_H_