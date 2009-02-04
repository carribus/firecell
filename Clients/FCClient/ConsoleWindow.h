#ifndef _CONSOLEWINDOW_H_
#define _CONSOLEWINDOW_H_

#include <string>
#include "FCModel.h"
#include "GUIConsoleCtrl.h"
#include "InGameAppWindow.h"

class ConsoleWindow :	public InGameAppWindow
										, public IGUIConsoleEventSink
{
public:
	ConsoleWindow(Desktop* pDesktop, FCController* pController, IrrlichtDevice* pDevice);
	~ConsoleWindow(void);

	bool Create(s32 AppElemID, FCUINT optionID, std::wstring caption);

  const wchar_t* getAppName()                           { return L"Console"; }

  void OnFileSystemInfoReceived(FCModel::FileSystemInfo* pFSI);
  void OnConsoleCommandResponse(std::string currentDir, std::string result);

	void SetCurrentDir(const std::string& curDir);
	void SetDirSeperator(const std::string& sep)					{ m_dirSeperator = sep; }
	void SetFileSystemStyle(FCBYTE style)									{ m_fsStyle = style; }

  void SetWaitingForResponse(bool bWait);

	/*
	 *	IGUIConsoleEventSink implementation
	 */
	void OnConsoleInputEvent(std::wstring input);

private:

	bool IsLocalCommand(const std::wstring cmd);
	void HandleLocalCommand(const std::wstring cmd);

	IrrlichtDevice*				m_pDevice;
	GUIConsoleCtrl* 			m_pLogWnd;
	std::string						m_currentDir;
	std::string						m_dirSeperator;
  FCBYTE								m_fsStyle;
  std::vector<wstring>  m_log;
};

#endif//_CONSOLEWINDOW_H_