#ifndef _GUIFCCHARACTERITEM_H_
#define _GUIFCCHARACTERITEM_H_

#include <irrlicht.h>
#include "Character.h"
#include "FCView.h"

using namespace irr;
using namespace gui;
using namespace video;

class GUIFCCharacterItem : public IGUIElement
{
public:
	GUIFCCharacterItem(IGUIEnvironment* pEnv, IGUIElement* pParent = NULL, s32 id = -1);
	~GUIFCCharacterItem(void);

	/*
	 *	IGUIElement overrides
	 */
	void draw();
  bool OnEvent(const SEvent& event);

	/*
	 *	Public Methods
	 */
  void setContainer(FCView* pContainer)                           { m_pContainer = pContainer; }
  void SetCharacter(Character* pCharacter)                        { m_pCharacter = pCharacter; }
	Character* GetCharacter()																				{ return m_pCharacter; }
  void SetHighlight(bool bHighlight)                              { m_bHighlight = bHighlight; }

private:

  IVideoDriver*              m_pDriver;
  FCView*                    m_pContainer;
  Character*                 m_pCharacter;
  bool                       m_bHighlight;
};

#endif//_GUIFCCHARACTERITEM_H_