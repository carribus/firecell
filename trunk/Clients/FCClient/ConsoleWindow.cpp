#include <sstream>
#include "../common/clienttypes.h"
#include "FCController.h"
#include "FCViewEvent.h"
#include "ConsoleWindow.h"

#define CONSOLE_CONTROL				1

ConsoleWindow::ConsoleWindow(IDesktop* pDesktop, FCController* pController, IrrlichtDevice* pDevice)
: InGameAppWindow(pDesktop, pController, pDevice->getGUIEnvironment())
, m_pDevice(pDevice)
, m_pLogWnd(NULL)
{
}

///////////////////////////////////////////////////////////////////////

ConsoleWindow::~ConsoleWindow(void)
{
}

///////////////////////////////////////////////////////////////////////

bool ConsoleWindow::Create(s32 AppElemID, FCUINT optionID, std::wstring caption)
{
	bool bResult = InGameAppWindow::Create(AppElemID, optionID, DOT_Console, caption);
	core::rect<s32> wndRect(0, 0, 640, 480), clientRect;

	if ( bResult )
	{
		m_pWindow->setRelativePosition(wndRect);
    CenterWindow();
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
			m_pLogWnd->drop();
      // request the OS version string from the server
      OnConsoleInputEvent(L"ver");
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

	if ( !IsLocalCommand(input) )
	{
		ConsoleCommand cc;
		char buffer[1024];

		snprintf(buffer, sizeof(buffer)-1, "%S", input.c_str());
		cc.command = buffer;
		cc.currentDir = m_currentDir;

		SetWaitingForResponse(true);
		FCViewEvent e(VE_ConCommandIssued, (long long)&cc);
		m_pController->OnViewEvent(e);
	}
	else
	{
		HandleLocalCommand(input);
	}
}

///////////////////////////////////////////////////////////////////////

bool ConsoleWindow::IsLocalCommand(const std::wstring cmd)
{
  wchar_t* localCmds[] =
  {
    L"exit",
    L"pwd",
    NULL
  };
  wchar_t** pCmd = localCmds;

  while ( *pCmd )
  {
    if ( !cmd.compare(*pCmd) )
      return true;
    pCmd++;
  }

  return false;
}

///////////////////////////////////////////////////////////////////////

void ConsoleWindow::HandleLocalCommand(const std::wstring cmd)
{
	std::wstring c = cmd;
  size_t pos = cmd.find(L" ");

	if ( pos != std::wstring::npos )
    c = cmd.substr(0, pos);

  if ( !c.compare(L"exit") )
  {
    // close the console window
    if ( m_pWindow->getParent() )
    {
		  SEvent e;
		  e.EventType = EET_GUI_EVENT;
		  e.GUIEvent.Caller = m_pWindow;
		  e.GUIEvent.Element = 0;
		  e.GUIEvent.EventType = EGET_ELEMENT_CLOSED;
      m_pWindow->getParent()->OnEvent(e);
    }
  }
  else if ( !c.compare(L"pwd") )
  {
  }
}