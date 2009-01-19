#include <sstream>
#include "FCController.h"
#include "FCViewEvent.h"
#include "ConsoleWindow.h"

#define CWID_STATIC_LOG			1

ConsoleWindow::ConsoleWindow(FCController* pController, IGUIEnvironment* pEnv)
: InGameAppWindow(pController, pEnv)
, m_pLogWnd(NULL)
{
}

///////////////////////////////////////////////////////////////////////

ConsoleWindow::~ConsoleWindow(void)
{
}

///////////////////////////////////////////////////////////////////////

bool ConsoleWindow::Create(FCUINT optionID, std::wstring caption)
{
	bool bResult = InGameAppWindow::Create(optionID, DOT_Console, caption);
	core::rect<s32> wndRect(0, 0, 500, 400), clientRect;

	if ( bResult )
	{
		m_pWindow->setRelativePosition(wndRect);
		GetClientRect(clientRect);
		m_pLogWnd = m_pEnv->addStaticText(L"", clientRect, false, false, m_pWindow, CWID_STATIC_LOG);
    m_pWindow->addChild( m_pLogWnd );
		m_pLogWnd->setBackgroundColor( video::SColor(255, 0, 0, 0) );
		m_pLogWnd->setOverrideColor( video::SColor(255, 0, 196, 0) );

    // create the font
    IGUIFont* pFont = m_pEnv->getFont("./clientdata/fonts/fontfixedsys.xml");
		m_pLogWnd->setOverrideFont( pFont );
    pFont->drop();

		// request a refresh
    m_bWaitingForResponse = true;
		FCViewEvent e(VE_ConRefresh);
		m_pController->OnViewEvent(e);
	}

	return bResult;
}	

///////////////////////////////////////////////////////////////////////

void ConsoleWindow::OnFileSystemInfoReceived(FCModel::FileSystemInfo* pFSI)
{
  if ( !pFSI )
    return;

  m_currentDir = pFSI->currentDir;
  m_dirSeperator = pFSI->dirSeperator;
  m_fsStyle = pFSI->fsStyle;

  AddPrompt();
  Update();

  m_bWaitingForResponse = false;
}

///////////////////////////////////////////////////////////////////////

void ConsoleWindow::AddPrompt()
{
  std::wstringstream s;

  s << m_currentDir.c_str() << L" > ";
  m_log.push_back(s.str());
}

///////////////////////////////////////////////////////////////////////

void ConsoleWindow::Update()
{
  wstring str;
  std::vector<wstring>::iterator it = m_log.begin();
  std::vector<wstring>::iterator limit = m_log.end();

  for ( ; it != limit; it++ )
  {
    str += *it;
  }

  m_pLogWnd->setText(str.c_str());
}