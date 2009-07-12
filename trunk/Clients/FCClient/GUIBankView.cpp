#include <sstream>
#include "../common/fccutils.h"
#include "GUIBankView.h"

GUIBankView::GUIBankView(IGUIEnvironment* environment, core::rect<s32>& rect, IGUIElement* pParent, s32 id)
: IGUIElement(EGUIET_ELEMENT, environment, pParent, id, rect)
, m_pTxtDetailsLabels(NULL)
, m_pTxtDetails(NULL)
{
#ifdef _DEBUG
  setDebugName("GUIBankView");
#endif//_DEBUG

  m_pTxtDetailsLabels = environment->addStaticText(L"", core::rect<s32>( 10, 10, rect.getWidth()/2, 80 ), false, true, this, -1, false);
  m_pTxtDetails = environment->addStaticText(L"", core::rect<s32>( rect.getWidth()/2+1, 10, rect.getWidth()-10, 80 ), false, true, this, -1, false);

	// setup the font
	IGUIFont* pFont = environment->getFont("./clientdata/fonts/fontcourier.png");
	environment->getSkin()->setFont(pFont);

  m_pTxtDetailsLabels->setOverrideColor( SColor(255, 196, 196, 255) );
	m_pTxtDetailsLabels->setOverrideFont(pFont);
  m_pTxtDetailsLabels->setTextAlignment( EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT );
  m_pTxtDetails->setOverrideColor( SColor(255, 196, 196, 255) );
	m_pTxtDetails->setOverrideFont(pFont);

  // create the buttons
  core::rect<s32> bottomRect = core::rect<s32>( 10, 85, rect.getWidth()-10, rect.getHeight()-10 );
  core::rect<s32> btnRect = bottomRect;

  btnRect.LowerRightCorner.Y = bottomRect.UpperLeftCorner.Y + 20;
  btnRect.LowerRightCorner.X = bottomRect.UpperLeftCorner.X + 100;
  environment->addButton(btnRect, this, 1, L"Transfer");

}

///////////////////////////////////////////////////////////////////////

GUIBankView::~GUIBankView(void)
{
}

///////////////////////////////////////////////////////////////////////

void GUIBankView::updateAccountDetails(BankAccount* pAccount)
{
  if ( !pAccount )
    return;

  std::wstringstream ss;

  ss << formatCurrencyValue( "$", pAccount->getBalance() ).c_str() << "\n" << 
        formatCurrencyValue( "$", pAccount->getDebt() ).c_str() << "\n" << 
        pAccount->getInterestRate() << "%\n" << (pAccount->isSecure() ? "Yes" : "No");

  m_pTxtDetailsLabels->setText(L"Balance :\nDebt :\nInterest Rate :\nIs secure :\n");
  m_pTxtDetails->setText(ss.str().c_str());
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
