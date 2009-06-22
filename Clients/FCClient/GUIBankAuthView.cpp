#include "../common/ResourceManager.h"
#include "clientstrings.h"
#include "GUIBankAuthView.h"

GUIBankAuthView::GUIBankAuthView(IGUIEnvironment* environment, core::rect<s32>& rect, IGUIElement* pParent, s32 id)
: IGUIElement(EGUIET_ELEMENT, environment, pParent, id, rect)
{
#ifdef _DEBUG
  setDebugName("GUIBankAuthView");
#endif//_DEBUG

  // create the elements on the form

  core::rect<s32> boundingRect;
  IGUIStaticText* pTxt = NULL;
  IGUIElement* pElem = NULL;

  pTxt = environment->addStaticText( L"Your bank account requires authentication!",
                                     core::rect<s32>(0, 0, rect.getWidth(), 30),
                                     false,
                                     true,
                                     this);
  pTxt->setTextAlignment( EGUIA_CENTER, EGUIA_CENTER );
  pTxt->setOverrideColor( SColor(255, 255, 255, 255) );

  pTxt = environment->addStaticText( ResourceManager::instance().GetClientString( STR_LOGINWND_PASSWORD ).c_str(),
                                     core::rect<s32>(10, 40, 100, 60),
                                     false,
                                     false,
                                     this );
  pTxt->setTextAlignment( EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT );
  pTxt->setOverrideColor( SColor(255, 128, 128, 255) );

  boundingRect = pTxt->getRelativePosition();

  pElem = environment->addEditBox( L"",
                                   core::rect<s32>(105, 38, 280, 61),
                                   true,
                                   this,
                                   1);
}

///////////////////////////////////////////////////////////////////////

GUIBankAuthView::~GUIBankAuthView(void)
{
}

///////////////////////////////////////////////////////////////////////

void GUIBankAuthView::draw()
{
  if ( !IsVisible )
    return;
  
  IVideoDriver* pVideo = Environment->getVideoDriver();
  SColor  backCol = SColor(255, 16, 16, 32);

  pVideo->draw2DRectangle( AbsoluteRect, backCol, backCol, backCol, backCol );

	IGUIElement::draw();
}

///////////////////////////////////////////////////////////////////////

bool GUIBankAuthView::OnEvent(const SEvent& event)
{
  bool bResult = false;

  return bResult;
}
