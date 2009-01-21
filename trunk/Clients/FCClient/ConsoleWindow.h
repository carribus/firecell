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
	ConsoleWindow(FCController* pController, IrrlichtDevice* pDevice);
	~ConsoleWindow(void);

	bool Create(FCUINT optionID, std::wstring caption);

  void OnFileSystemInfoReceived(FCModel::FileSystemInfo* pFSI);

	void SetCurrentDir(const std::string& curDir)					{ m_currentDir = curDir; }
	void SetDirSeperator(const std::string& sep)					{ m_dirSeperator = sep; }
	void SetFileSystemStyle(FCBYTE style)									{ m_fsStyle = style; }

	/*
	 *	IGUIConsoleEventSink implementation
	 */
	void OnConsoleInputEvent(std::wstring input);

private:

	IrrlichtDevice*				m_pDevice;
	GUIConsoleCtrl* 			m_pLogWnd;
	std::string						m_currentDir;
	std::string						m_dirSeperator;
  FCBYTE								m_fsStyle;
  std::vector<wstring>  m_log;
};

#endif//_CONSOLEWINDOW_H_