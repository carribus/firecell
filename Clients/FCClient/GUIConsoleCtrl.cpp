#include "FCGUIElementFactory.h"
#include "GUIConsoleCtrl.h"

GUIConsoleCtrl::GUIConsoleCtrl(IGUIEnvironment*& environment, core::rect<s32> rect, IGUIElement* pParent, SColor backColor, SColor textColor, s32 id)
: IGUIElement((EGUI_ELEMENT_TYPE)FCGUIElementFactory::EGUIET_CONSOLE, environment, pParent, id, rect)
, m_pTimer(NULL)
, m_backColor(backColor)
, m_textColor(textColor)
, m_pOverrideFont(NULL)
, m_bNeedRecalc(true)
, m_bFreezeConsole(false)
, m_bCaretVisible(true)
, m_maxLogSize(5000)
, m_fontHeight(0)
, m_maxVisibleLines(0)
, m_posInput(0)
, m_historySize(10)
, m_historyIndex(0)
, m_pEventSink(NULL)
{
	#ifdef _DEBUG
	setDebugName("GUIConsoleCtrl");
	#endif
}

///////////////////////////////////////////////////////////////////////

GUIConsoleCtrl::~GUIConsoleCtrl(void)
{
}

///////////////////////////////////////////////////////////////////////

void GUIConsoleCtrl::draw()
{
  std::wstring line;
  IGUIFont* pFont = m_pOverrideFont ? m_pOverrideFont : Environment->getSkin()->getFont();

	if ( m_pOverrideFont )
		pFont = m_pOverrideFont;

  // check if we need to recalculate our metrics and dimensions
  if ( m_bNeedRecalc )
  {
    core::dimension2d<s32> dim = pFont->getDimension(L"ABCDEFG");
    m_fontHeight = dim.Height;
    m_maxVisibleLines = AbsoluteRect.getHeight() / m_fontHeight;
    m_bNeedRecalc = false;
  }

  // draw the background
  Environment->getVideoDriver()->draw2DRectangle( AbsoluteRect, m_backColor, m_backColor, m_backColor, m_backColor );

  // draw the text log
  DrawLog(pFont);

  // call base drawing code to draw children
  IGUIElement::draw();
}

///////////////////////////////////////////////////////////////////////

void GUIConsoleCtrl::DrawLog(IGUIFont* pFont)
{
  core::dimension2d<s32> extents;
  core::rect<s32> rectTxt = AbsoluteRect;
  std::wstring subStr;
  std::vector<stLineMarker>::iterator it = m_arrLineMarkers.begin();
  std::vector<stLineMarker>::iterator limit = m_arrLineMarkers.end();

  // calculate which line we should be starting with (drawing top to bottom)
  size_t lmCount = m_arrLineMarkers.size();
  if ( lmCount > 0 )
  {
    s32 ndx = 0;
    size_t pCount = m_arrLineMarkersPrompt.size();
    if ( m_maxVisibleLines > (s32)pCount )
    {
      ndx = (s32)(lmCount - m_maxVisibleLines + pCount);
      ndx = ndx > 0 ? ndx : 0;
    }
    else
    {
      ndx = lmCount;
    }
    it += ndx;
  }

  for ( ; it != limit; ++it )
  {
    subStr = m_strLog.substr( it->startPos, it->endPos - it->startPos );
    extents = pFont->getDimension( subStr.c_str() );

    rectTxt.LowerRightCorner.X = rectTxt.UpperLeftCorner.X + extents.Width;
    rectTxt.LowerRightCorner.Y = rectTxt.UpperLeftCorner.Y + extents.Height;

		// draw the text line
		pFont->draw( subStr.c_str(), rectTxt, m_textColor, false, false, &rectTxt );
		// offset the text rect
		rectTxt.LowerRightCorner += core::position2d<s32>(0, extents.Height);
		rectTxt.UpperLeftCorner += core::position2d<s32>(0, extents.Height);
  }

  if ( !m_bFreezeConsole )
  {
    DrawPrompt(pFont, rectTxt);
  }
}

///////////////////////////////////////////////////////////////////////

void GUIConsoleCtrl::DrawPrompt(IGUIFont* pFont, core::rect<s32> rectTxt)
{
  static u32 lastTick = 0;
  core::dimension2d<s32> extents;
  std::wstring str = m_prompt + m_input, subStr;

  std::vector<stLineMarker>::iterator it = m_arrLineMarkersPrompt.begin();
  std::vector<stLineMarker>::iterator limit = m_arrLineMarkersPrompt.end();

  s32 ndx = 0;
  size_t pCount = m_arrLineMarkersPrompt.size();
  if ( m_maxVisibleLines < (s32)pCount )
  {
    ndx = (s32)(pCount - m_maxVisibleLines);
    it += ndx;
  }

  rectTxt.LowerRightCorner.X = rectTxt.UpperLeftCorner.X;
  for ( ; it != limit; )
  {
    subStr = str.substr( it->startPos, it->endPos - it->startPos );
    extents = pFont->getDimension( subStr.c_str() );

    rectTxt.LowerRightCorner.X = rectTxt.UpperLeftCorner.X + extents.Width;
    rectTxt.LowerRightCorner.Y = rectTxt.UpperLeftCorner.Y + extents.Height;

		// draw the text line
		pFont->draw( subStr.c_str(), rectTxt, m_textColor, false, false, &rectTxt );
    // get the next linemarker
    ++it;
		// offset the text rect
    if ( it != limit )
    {
	    rectTxt.LowerRightCorner += core::position2d<s32>(0, extents.Height);
	    rectTxt.UpperLeftCorner += core::position2d<s32>(0, extents.Height);
    }
  }
/*
  // draw the prompt
  if ( m_prompt.size() )
  {
    extents = pFont->getDimension( m_prompt.c_str() );
    rectTxt.LowerRightCorner.X = rectTxt.UpperLeftCorner.X + extents.Width;
    rectTxt.LowerRightCorner.Y = rectTxt.UpperLeftCorner.Y + extents.Height;
    // draw the text line
    pFont->draw( m_prompt.c_str(), rectTxt, m_textColor, false, false, &rectTxt );
  }

  // if there is any input in the buffer, then draw it
  if ( m_input.size() )
  {
    extents = pFont->getDimension( m_input.c_str() );
    rectTxt.UpperLeftCorner.X = rectTxt.LowerRightCorner.X;
    rectTxt.LowerRightCorner.X = rectTxt.UpperLeftCorner.X + extents.Width;
    pFont->draw( m_input.c_str(), rectTxt, m_textColor, false, false, &rectTxt );
  }
*/
  // draw a caret
  if ( m_pTimer && Environment->hasFocus(this) )
  {
    u32 now = m_pTimer->getTime();
    if ( now - lastTick > 500 )
    {
      // Toggle the caret's visibility
      m_bCaretVisible ^= true;
      lastTick = now;
    }

    if ( m_bCaretVisible )
    {
      extents = pFont->getDimension( L"_" );
      DrawCaret( rectTxt );
    }
  }
}

///////////////////////////////////////////////////////////////////////

bool GUIConsoleCtrl::OnEvent(const SEvent& event)
{
  bool bResult = false;

  switch ( event.EventType )
  {
	case	EET_GUI_EVENT:
		{
			switch ( event.GUIEvent.EventType )
			{
			case	EGET_ELEMENT_FOCUSED:
        {
          static bool bIgnoreFocus = false;
				  m_bCaretVisible = true;
          if ( Parent )
          {
            if ( !bIgnoreFocus )              // avoid re-entry
            {
              Environment->setFocus(Parent);
              bIgnoreFocus = true;
              Environment->setFocus(this);
              bIgnoreFocus = false;
              
              AnalysePrompt();
            }
          }
        }
				break;

			default:
				break;
			}
		}
		break;

  case  EET_KEY_INPUT_EVENT:
    bResult = ProcessKeyInput(event);
    break;

	default:
		break;
  }

  return bResult;
}

///////////////////////////////////////////////////////////////////////

u32 GUIConsoleCtrl::addTextLine(const std::wstring& line)
{
  m_strLog.append(line);
  m_strLog.append(L"\n");
  AnalyseLog(m_strLog, m_arrLineMarkers);
  return 0;
}

///////////////////////////////////////////////////////////////////////

void GUIConsoleCtrl::AnalyseLog(std::wstring& src, std::vector<stLineMarker>& targetArray, bool bClearArray)
{
  IGUIFont* pFont = (m_pOverrideFont ? m_pOverrideFont : Environment->getSkin()->getFont());
  core::rect<s32> rectTxt = AbsoluteRect;
  core::dimension2di extents;
  s32 curX = rectTxt.UpperLeftCorner.X, curY = rectTxt.UpperLeftCorner.Y;
  s32 rectWidth = AbsoluteRect.getWidth(), rectHeight = AbsoluteRect.getHeight();
  static wchar_t* SEPERATORS = L" ,.-|\\/\n";

  size_t pos = 0, pos2 = 0, lineStartPos = 0;
  std::wstring subStr;

  if ( bClearArray )
  {
    ClearLineMarkers(targetArray);
  }
  else if ( targetArray.size() )
  {
    stLineMarker lm = targetArray.back();
    pos2 = lineStartPos = lm.endPos;
  }

  pos = src.find_first_of(SEPERATORS, pos2);

  while ( pos != src.npos )
  {
    // get the word
    subStr = src.substr(pos2, pos-pos2+1);
    // calculate the extents of the word text
    extents = pFont->getDimension(subStr.c_str());

    // check if the word would extend past the right edge of the control
    if ( curX + extents.Width <= rectTxt.LowerRightCorner.X )
    {
      // text fits... move along
      curX += extents.Width;
      pos2 = pos+1;
    }
    else
    {
      if ( lineStartPos == pos2 )
      {
        // check if we have an edge case. This could be caused by a reallly long word being entered without breaks across one visible line.
        // in this case, we need to force a break at the first letter that will not overflow
        while ( curX + extents.Width > rectTxt.LowerRightCorner.X )
        {
          subStr.erase( subStr.size()-1, 1 );
          pos--;
          extents = pFont->getDimension(subStr.c_str());
        }
        pos2 = pos;
      }
      // create the 'next line marker'
      CreateLineMarker(targetArray, lineStartPos, pos2);
      lineStartPos = pos2;
      // move to the next line
      curY += extents.Height;
      curX = rectTxt.UpperLeftCorner.X;
    }

    // get the next word
    pos = src.find_first_of(SEPERATORS, pos2);
  }

  // clear out the trailing 'seperator' that we added to simplify the algorithm
  if ( src.size() && src.find_first_of(SEPERATORS, src.size()-1) == src.size()-1 )
    src.erase( src.size()-1, 1 );

  if ( pos == m_strLog.npos && lineStartPos < src.size() )
  {
    CreateLineMarker( targetArray, lineStartPos, src.size() );
  }
  else
  {
    // check if there is any residual text for the next line
    if ( pos2 == src.size() && curX != rectTxt.UpperLeftCorner.X && lineStartPos < pos2 )
      CreateLineMarker( targetArray, lineStartPos, pos2 );
  }
}

///////////////////////////////////////////////////////////////////////

void GUIConsoleCtrl::AnalysePrompt()
{
  std::wstring str = m_prompt + m_input + L" ";
  AnalyseLog(str, m_arrLineMarkersPrompt, true);
}

///////////////////////////////////////////////////////////////////////

void GUIConsoleCtrl::CreateLineMarker(std::vector<stLineMarker>& targetArray, size_t startPos, size_t endPos)
{
  stLineMarker lm;

  lm.startPos = startPos;
  lm.endPos = endPos;
  targetArray.push_back(lm);
}

///////////////////////////////////////////////////////////////////////

void GUIConsoleCtrl::ClearLineMarkers(std::vector<stLineMarker>& targetArray)
{
  targetArray.clear();
}

///////////////////////////////////////////////////////////////////////

void GUIConsoleCtrl::DrawCaret(const core::rect<s32>& rect)
{
  IGUIFont* pFont = Environment->getSkin()->getFont();
  core::rect<s32> r = rect;

  if ( m_posInput != m_input.size() )
  {
    core::dimension2d<s32> extents = pFont->getDimension( m_input.substr(0, m_posInput).c_str() );
    r.LowerRightCorner.X = r.UpperLeftCorner.X + extents.Width;
  }

  pFont->draw( L"_", core::rect<s32>( r.LowerRightCorner.X, r.UpperLeftCorner.Y, r.LowerRightCorner.X, r.LowerRightCorner.Y ), m_textColor );
}

///////////////////////////////////////////////////////////////////////

bool GUIConsoleCtrl::ProcessKeyInput(const SEvent& event)
{
  if ( !event.KeyInput.PressedDown )
    return false;

  bool bResult = false;

  switch ( event.KeyInput.Key )
  {
  case  KEY_RETURN:
    {
      // add the prompt and text line to the log
      addTextLine( m_prompt + m_input );
      // add the input the history log
      m_arrHistory.push_back(m_input);
      if ( m_arrHistory.size() > m_historySize )
        m_arrHistory.erase( m_arrHistory.begin() );
      m_historyIndex = (u32)m_arrHistory.size();
      // if we have an event sink registered, then we should fire a notification with the input text here
			if ( m_pEventSink )
				m_pEventSink->OnConsoleInputEvent(m_input);
      // clear the input text and reset the caret position
      m_input.clear();
      m_posInput = 0;

      bResult = true;
    }
    break;

  case  KEY_DELETE:   // delete the character to the right of the caret
    {
      if ( m_posInput < m_input.size() )
      {
        m_input.erase( m_posInput, 1 );
      }

      bResult = true;
    }
    break;

  case  KEY_LEFT:   // move the caret to the left by one character
    {
      if ( m_posInput )
        m_posInput--;

      bResult = true;
    }
    break;

  case  KEY_RIGHT:  // move the caret to the right by one character
    {
      if ( m_posInput < m_input.size() )
        m_posInput++;

      bResult = true;
    }
    break;

  case  KEY_UP:   // This goes backwards through the history log of inputs
    {
      if ( m_historyIndex == 0 )
        m_historyIndex = (u32)( (u32)m_arrHistory.size() < m_historySize ? m_arrHistory.size()-1 : m_historySize-1);
      else
        m_historyIndex--;
      m_input = m_arrHistory[m_historyIndex];
      m_posInput = (u32)m_input.size();

      bResult = true;
    }
    break;

  case  KEY_DOWN:   // move forward through the history log of inputs
    {
      if ( m_historyIndex >= m_arrHistory.size()-1 )
        m_historyIndex = 0;
      else
        m_historyIndex++;
      m_input = m_arrHistory[m_historyIndex];
      m_posInput = (u32)m_input.size();

      bResult = true;
    }
    break;

  case  KEY_SHIFT:
    break;

  case  KEY_BACK: // erase the character to the left of the caret
    {
      if ( m_posInput )
      {
        m_input.erase( --m_posInput, 1 );
      }

      bResult = true;
    }
    break;

  case  KEY_F1:
  case  KEY_F2:
  case  KEY_F3:
  case  KEY_F4:
  case  KEY_F5:
  case  KEY_F6:
  case  KEY_F7:
  case  KEY_F8:
  case  KEY_F9:
  case  KEY_F10:
  case  KEY_F11:
  case  KEY_F12:
  case  KEY_F13:
  case  KEY_F14:
  case  KEY_F15:
  case  KEY_F16:
  case  KEY_F17:
  case  KEY_F18:
  case  KEY_F19:
  case  KEY_F20:
  case  KEY_F21:
  case  KEY_F22:
  case  KEY_F23:
  case  KEY_F24:
    break;

  default:
    {
      m_input.insert(m_posInput, &event.KeyInput.Char, 1);
      m_posInput ++;
      bResult = true;
    }
    break;
  }

  {
    std::wstring str = m_prompt + m_input + L" ";
    AnalysePrompt();
  }

  return bResult;
}

///////////////////////////////////////////////////////////////////////

void GUIConsoleCtrl::setPrompt(const std::wstring& prompt)
{
  m_prompt = prompt;
  AnalysePrompt();
}

///////////////////////////////////////////////////////////////////////

void GUIConsoleCtrl::setHistoryLogSize(u32 size)
{
  m_historySize = size;
  while ( m_arrHistory.size() > size )
  {
    m_arrHistory.erase( m_arrHistory.begin() );
  }

  if ( m_historyIndex >= size )
    m_historyIndex = size-1;
}

///////////////////////////////////////////////////////////////////////

void GUIConsoleCtrl::setMaxLogSize(u32 size)
{
  m_maxLogSize = size;
  while ( m_arrLineMarkers.size() > size )
  {
    m_arrLineMarkers.erase( m_arrLineMarkers.begin() );
  }
}