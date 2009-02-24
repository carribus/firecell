#ifndef _TextAnimator_H_
#define _TextAnimator_H_

#include <string>
#include <vector>
#include <irrlicht.h>
#include "../../common/PThreadRWLock.h"

using namespace irr;
using namespace gui;

////////////////////////////////////////////////////////////////////////////////////////

class AnimatedText
{
public:
	AnimatedText(IGUIFont* pFont, const std::wstring& text, video::SColor textCol, core::rect<s32> rect)
		: m_pFont(pFont)
		, m_text(text)
		, m_textCol(textCol)
		, m_rect(rect)
	{
		if ( m_pFont )
			m_pFont->grab();
	}

	virtual ~AnimatedText()
	{
		if ( m_pFont )
			m_pFont->drop();
	}

	void draw()
	{
		m_pFont->draw( m_text.c_str(), m_rect, m_textCol, true, true );
	}

	void setText(const std::wstring& text)							{ m_text = text; }
	void setTextColor(video::SColor col)								{ m_textCol = col; }
	void setRect(core::rect<s32> rect)									{ m_rect = rect; }

	std::wstring& getText()															{ return m_text; }
	video::SColor getTextColor()												{ return m_textCol; }
	core::rect<s32> getRect()														{ return m_rect; }

private:

	IGUIFont*				m_pFont;
	std::wstring		m_text;
	video::SColor		m_textCol;
	core::rect<s32>	m_rect;
};

////////////////////////////////////////////////////////////////////////////////////////

class TextAnimator
{
public:

	typedef void (*TEXTANIM_CALLBACK)(IGUIElement*);

	enum E_TEXTANIM_DIR
	{
		TAD_VERTICAL,
		TAD_HORIZONTAL
	};

	enum E_TEXTANIM_EFFECT
	{
		TAE_NONE,
		TAE_FADEOUT,
		TAE_FADEIN
	};

	TextAnimator(void);
	~TextAnimator(void);

	/**
	 *	@brief Add an element to animate in a particular direction at a specific speed, with an end effect, and for a specific period of time
	 *	@param pText The text object to animate
	 *	@param dir The direction in which to move the element
	 *	@param offsetPerTick The number of pixels to offset the element per tick
	 *	@param delay The number of milliseconds between each 'tick'
	 *	@param effect The effect to apply to the element while animating
	 *	@param ticksBeforeEnd The number of ticks before the animation ends.
	 *	@param callBack An optional callback to invoke once the animation has reached its completion point
	 */
	void addTextToAnimate(AnimatedText* pText, E_TEXTANIM_DIR dir, s32 offsetPerTick, u32 delay, E_TEXTANIM_EFFECT effect, u32 ticksBeforeEffect, u32 ticksBeforeEnd, TEXTANIM_CALLBACK callBack = NULL);

	/**
	 *	@brief This function will iterate over all the text objects its managing and update their positions/display based on the animation parameters provided
	 */
	void refreshObjects();

	/**
	 *	@brief This function will render all the text objects
	 */
	void renderObjects();

	void setTimer(irr::ITimer* pTimer)												{ m_pTimer = pTimer; }

private:

	/*
	 *	Private structure(s)
	 */
	struct TextAnimObj
	{
		AnimatedText* pTextObj;
		E_TEXTANIM_DIR dir;
		s32 offsetPerTick;
		u32 delay;
		E_TEXTANIM_EFFECT effect;
		u32 ticksBeforeEffect;
		s32 effectDelta;
		u32 ticksBeforeEnd;
		TEXTANIM_CALLBACK callback;

		u32 lastTick;
		u32 numTicksAnimated;
	};

	/*
	 *	Private methods
	 */
	void updateObject(TextAnimObj& obj);

	/*
	 *	Private members
	 */	
	irr::ITimer*							m_pTimer;

	typedef std::vector<TextAnimObj> ElementVector;
	ElementVector m_objects;
	PThreadRWLock m_lockElems;
};

#endif//_TextAnimator_H_