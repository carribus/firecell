#include "GUIBankView.h"

GUIBankView::GUIBankView(IGUIEnvironment* environment, core::rect<s32>& rect, IGUIElement* pParent, s32 id)
: IGUIElement(EGUIET_ELEMENT, environment, pParent, id, rect)
, m_pModel(NULL)
{
#ifdef _DEBUG
  setDebugName("GUIBankView");
#endif//_DEBUG

  m_pModel = BankModel::instance();
}

///////////////////////////////////////////////////////////////////////

GUIBankView::~GUIBankView(void)
{
}

///////////////////////////////////////////////////////////////////////

void GUIBankView::draw()
{
  if ( !IsVisible )
    return;
  
  IVideoDriver* pVideo = Environment->getVideoDriver();
  SColor  backCol = SColor(255, 32, 32, 96);

  pVideo->draw2DRectangle( AbsoluteRect, backCol, backCol, backCol, backCol );

	IGUIElement::draw();
}

///////////////////////////////////////////////////////////////////////

bool GUIBankView::OnEvent(const SEvent& event)
{
  bool bResult = false;

  return bResult;
}
