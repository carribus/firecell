#include <sstream>
#include "../common/clienttypes.h"
#include "FCController.h"
#include "FCViewEvent.h"
#include "ConsoleWindow.h"

#define CONSOLE_CONTROL				1

ConsoleWindow::ConsoleWindow(FCController* pController, IrrlichtDevice* pDevice)
: InGameAppWindow(pController, pDevice->getGUIEnvironment())
, m_pDevice(pDevice)
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
	core::rect<s32> wndRect(0, 0, 500, 420), clientRect;

	if ( bResult )
	{
		m_pWindow->setRelativePosition(wndRect);
		GetClientRect(clientRect);

    m_pLogWnd = (GUIConsoleCtrl*)m_pEnv->addGUIElement("console", m_pWindow);
		if ( m_pLogWnd )
		{
			m_pLogWnd->setID(CONSOLE_CONTROL);
			m_pLogWnd->registerEventSink(this);
			m_pLogWnd->setRelativePosition( clientRect );
			m_pLogWnd->setBackgroundColor(SColor(255, 0, 0, 0));
			m_pLogWnd->setTextColor(SColor(255, 0, 255, 0));
			m_pLogWnd->setTimer( m_pDevice->getTimer() );
			m_pLogWnd->addTextLine(L"irrLicht GUIConsoleCtrl tester");
			m_pLogWnd->addTextLine(L"Written by carribus (c) 2009");
			m_pLogWnd->addTextLine(L"Another line of uselessnes....");
			m_pLogWnd->addTextLine(L"------------------------------");
			m_pLogWnd->drop();
		}

    // create the font
//    IGUIFont* pFont = m_pEnv->getFont("./clientdata/fonts/fontfixedsys.xml");
//		m_pLogWnd->setOverrideFont( pFont );

    // get focus
    m_pDevice->getGUIEnvironment()->setFocus( m_pLogWnd );

		// request a refresh
    SetWaitingForResponse(true);

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

  SetCurrentDir( pFSI->currentDir );
  m_dirSeperator = pFSI->dirSeperator;
  m_fsStyle = pFSI->fsStyle;

  SetWaitingForResponse(false);
}

///////////////////////////////////////////////////////////////////////

void ConsoleWindow::OnConsoleCommandResponse(std::string currentDir, std::string result)
{
  std::string temp;
  std::wstring line;
  std::wstringstream s;

  SetCurrentDir(currentDir);
  size_t pos = result.find_first_of("\n");

  while ( pos != std::string::npos )
  {
    temp = result.substr(0, pos);
    s << temp.c_str();
    line = s.str();
    m_pLogWnd->addTextLine(line);
    result.erase(0, pos+1);
    pos = result.find_first_of("\n");
    s.str(L"");
  }

  SetWaitingForResponse(false);
}

///////////////////////////////////////////////////////////////////////

void ConsoleWindow::SetCurrentDir(const std::string& curDir)
{
  m_currentDir = curDir;
  // update the prompt
  wstringstream s;
  s << m_currentDir.c_str() << L">";
  m_pLogWnd->setPrompt(s.str());
}

///////////////////////////////////////////////////////////////////////

void ConsoleWindow::SetWaitingForResponse(bool bWait)
{
  InGameAppWindow::SetWaitingForResponse(bWait);
  m_pLogWnd->freezeConsole(bWait);
}

///////////////////////////////////////////////////////////////////////

void ConsoleWindow::OnConsoleInputEvent(std::wstring input)
{
	if ( !input.size() )
		return;

	ConsoleCommand cc;
	char buffer[1024];

	snprintf(buffer, sizeof(buffer)-1, "%S", input.c_str());
	cc.command = buffer;
	cc.currentDir = m_currentDir;

  SetWaitingForResponse(true);
	FCViewEvent e(VE_ConCommandIssued, (long long)&cc);
	m_pController->OnViewEvent(e);
}

///////////////////////////////////////////////////////////////////////
