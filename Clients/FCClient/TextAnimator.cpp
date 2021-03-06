#include "../common/irrlichtUtil/irrutils.h"
#include "TextAnimator.h"

TextAnimator::TextAnimator(void)
: m_pTimer(NULL)
{
}

///////////////////////////////////////////////////////////////////////

TextAnimator::~TextAnimator(void)
{
}

///////////////////////////////////////////////////////////////////////

void TextAnimator::addTextToAnimate(AnimatedText* pText, E_TEXTANIM_DIR dir, s32 offsetPerTick, u32 delay, E_TEXTANIM_EFFECT effect, u32 ticksBeforeEffect, u32 ticksBeforeEnd, TEXTANIM_CALLBACK callBack)
{
	if ( !pText)
		return;

	TextAnimObj obj = { pText, dir, offsetPerTick, delay, effect, ticksBeforeEffect, 0, ticksBeforeEnd, callBack };
	obj.lastTick = m_pTimer->getTime();
	obj.numTicksAnimated = 0;

	switch ( effect )
	{
	case	TAE_NONE:
		obj.effectDelta = 0;
		break;

	case	TAE_FADEOUT:
		obj.effectDelta = -((s32)pText->getTextColor().getAlpha() / (s32)(ticksBeforeEnd-ticksBeforeEffect));
		break;

	case	TAE_FADEIN:
		obj.effectDelta = (255 - pText->getTextColor().getAlpha()) / (ticksBeforeEnd-ticksBeforeEffect);
		break;
	}

  // make sure that the text isn't overlapping any other animated text at this point
  fitTextIntoStream(obj);

	m_lockElems.LockForWrite();
	m_objects.push_back(obj);
	m_lockElems.Unlock();
}

///////////////////////////////////////////////////////////////////////

void TextAnimator::refreshObjects()
{
	m_lockElems.LockForRead();
	
	ElementVector::iterator it = m_objects.begin();
  ElementVector::iterator nextIt;
	u32 now = m_pTimer->getTime();

	for ( ; it != m_objects.end(); it++ )
	{
		if ( now - it->lastTick >= it->delay )
		{
			updateObject(*it);
			// update the last tick on the object
			it->lastTick = now;

			// check if we're done animating the object
			if ( it->numTicksAnimated >= it->ticksBeforeEnd )
			{
				delete it->pTextObj;
        if ( it != m_objects.begin() )
        {
				  m_objects.erase(it--);
        }
        else
        {
          m_objects.erase(it);
          it = m_objects.begin();
        }
        if ( it == m_objects.end() )
          break;
			}
		}
	}

	m_lockElems.Unlock();
}

///////////////////////////////////////////////////////////////////////

void TextAnimator::renderObjects()
{
	m_lockElems.LockForRead();

	ElementVector::iterator it = m_objects.begin();
	ElementVector::iterator limit = m_objects.end();

	for ( ; it != limit; it++ )
	{
		it->pTextObj->draw();
	}

	m_lockElems.Unlock();
}

///////////////////////////////////////////////////////////////////////

void TextAnimator::updateObject(TextAnimObj& obj)
{
	core::rect<s32> r = obj.pTextObj->getRect();
	video::SColor textCol;

	// offset the object as expected
	switch ( obj.dir )
	{
	case	TAD_VERTICAL:
		offsetRect(r, 0, obj.offsetPerTick);
		break;

	case	TAD_HORIZONTAL:
		offsetRect(r, obj.offsetPerTick, 0);
		break;
	}
	obj.pTextObj->setRect(r);

	// perform whatever effect we need to
	switch ( obj.effect )
	{
	case	TAE_NONE:
		break;

	case	TAE_FADEOUT:
		{
			if ( obj.ticksBeforeEffect <= obj.numTicksAnimated )
			{
				textCol = obj.pTextObj->getTextColor();
				textCol.setAlpha( textCol.getAlpha() + obj.effectDelta);
				obj.pTextObj->setTextColor(textCol);
			}
		}
		break;

	case	TAE_FADEIN:
		{
			if ( obj.ticksBeforeEffect >= obj.numTicksAnimated )
			{
				textCol = obj.pTextObj->getTextColor();
				textCol.setAlpha( textCol.getAlpha() + obj.effectDelta);
				obj.pTextObj->setTextColor(textCol);
			}
		}
		break;

	default:
		break;
	}

	obj.numTicksAnimated++;

}

///////////////////////////////////////////////////////////////////////

void TextAnimator::fitTextIntoStream(TextAnimObj& obj)
{
  if ( m_objects.size() )
  {
    m_lockElems.LockForRead();

    core::rect<s32> objRect;
    TextAnimObj& lastObj = m_objects[m_objects.size()-1];

    objRect = lastObj.pTextObj->getRect();
    if ( objRect.LowerRightCorner.Y > obj.pTextObj->getRect().UpperLeftCorner.Y )
    {
      objRect.UpperLeftCorner.Y = objRect.LowerRightCorner.Y;
      objRect.LowerRightCorner.Y = objRect.UpperLeftCorner.Y + obj.pTextObj->getRect().getHeight();
      obj.pTextObj->setRect(objRect);
    }

    m_lockElems.Unlock();
  }
}