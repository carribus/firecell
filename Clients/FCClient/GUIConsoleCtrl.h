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

class GUIConsoleCtrl : public IGUIElement
{
public:
  GUIConsoleCtrl(IGUIEnvironment* environment, core::rect<s32>& rect, IGUIElement* pParent = 0, SColor backColor = SColor(255, 0, 0, 0), SColor textColor = SColor(255, 128, 128, 128), s32 id = -1);
  ~GUIConsoleCtrl(void);

  /*
   *  IGUIElement overrides
   */
  void draw();
  bool OnEvent(const SEvent& event);

  u32 addTextLine(const std::wstring& line)                     { m_arrLogLines.push_back(line); return (u32)m_arrLogLines.size(); }
  void setBackgroundColor(SColor col)                           { m_backColor = col; }
  void setTextColor(SColor col)                                 { m_textColor = col; }
  SColor getBackgroundColor()                                   { return m_backColor; }
  SColor getTextColor()                                         { return m_textColor; }
  void setTimer(ITimer* pTimer)                                 { m_pTimer = pTimer; }
  void setPrompt(const std::wstring& prompt)                    { m_prompt = prompt; }

protected:

  void DrawCaret(const core::rect<s32>& rect);
  bool ProcessKeyInput(const SEvent& event);

  ITimer*           m_pTimer;
  SColor            m_backColor;
  SColor            m_textColor;

  bool              m_bCaretVisible;
  std::vector<std::wstring>   m_arrLogLines;

  std::wstring      m_prompt;
  std::wstring      m_input;
  u32               m_posInput;                     // position of the caret in the input string
};

} // end of namespace gui

} // end of namespace irr

#endif// _GUICONSOLECTRL_H_