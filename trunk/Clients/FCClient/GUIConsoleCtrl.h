#ifndef _GUICONSOLECTRL_H_
#define _GUICONSOLECTRL_H_

#include <string>
#include <vector>
#include <irrlicht.h>

namespace irr
{

using namespace video;

namespace gui
{

struct IGUIConsoleEventSink
{
	virtual void OnConsoleInputEvent(std::wstring input) = 0;
};

class GUIConsoleCtrl : public IGUIElement
{
public:
  GUIConsoleCtrl(IGUIEnvironment*& environment, core::rect<s32> rect, IGUIElement* pParent = 0, SColor backColor = SColor(255, 0, 0, 0), SColor textColor = SColor(255, 128, 128, 128), s32 id = -1);
  ~GUIConsoleCtrl(void);

  /*
   *  IGUIElement overrides
   */
  void draw();
  bool OnEvent(const SEvent& event);

	void registerEventSink(IGUIConsoleEventSink* pSink)						{ m_pEventSink = pSink; }
  void freezeConsole(bool bFreeze)                              { m_bFreezeConsole = bFreeze; }
  bool isConsoleFrozen()                                        { return m_bFreezeConsole; }

  u32 addTextLine(const std::wstring& line)                     { m_arrLogLines.push_back(line); return (u32)m_arrLogLines.size(); }
  void setBackgroundColor(SColor col)                           { m_backColor = col; }
  void setTextColor(SColor col)                                 { m_textColor = col; }
  SColor getBackgroundColor()                                   { return m_backColor; }
  SColor getTextColor()                                         { return m_textColor; }
  void setTimer(ITimer* pTimer)                                 { m_pTimer = pTimer; }
  void setPrompt(const std::wstring& prompt)                    { m_prompt = prompt; }
  void setHistoryLogSize(u32 size);
  u32 getHistoryLogSize()                                       { return m_historySize; }
  void setMaxLogSize(u32 size);
  u32 getMaxLogSize()                                           { return m_maxLogSize; }
	void setOverrideFont(IGUIFont* pFont)													{ m_pOverrideFont = pFont; }

protected:

  void DrawLog(IGUIFont* pFont);
  void DrawPrompt(IGUIFont* pFont, core::rect<s32> rectTxt);
  void DrawCaret(const core::rect<s32>& rect);
  bool ProcessKeyInput(const SEvent& event);

  ITimer*           m_pTimer;
  SColor            m_backColor;
  SColor            m_textColor;
	IGUIFont*					m_pOverrideFont;

  bool              m_bNeedRecalc;
  bool              m_bFreezeConsole;
  bool              m_bCaretVisible;
  u32               m_maxLogSize;
  std::vector<std::wstring>   m_arrLogLines;

  s32               m_fontHeight;
  s32               m_maxVisibleLines;

  std::wstring      m_prompt;
  std::wstring      m_input;
  u32               m_posInput;                     // position of the caret in the input string
  u32               m_historySize;                  // size of input history backlog
  u32               m_historyIndex;
  std::vector<std::wstring> m_arrHistory;

	IGUIConsoleEventSink*	m_pEventSink;
};

} // end of namespace gui

} // end of namespace irr

#endif// _GUICONSOLECTRL_H_