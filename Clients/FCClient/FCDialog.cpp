#include "FCDialog.h"

FCDialog::FCDialog(IGUIEnvironment* env, IGUIElement* pParent, wchar_t* caption, bool bModal, s32 id, core::rect<s32> rect)
: IGUIElement(EGUIET_ELEMENT, env, pParent ? pParent : env->getRootGUIElement(), id, rect)
, m_pEnv(env)
, m_bModal(bModal)
, Dragging(false)
, m_fpSuccessCallback(NULL)
, m_fpCancelCallback(NULL)
, m_pSuccessCtx(NULL)
, m_pCancelCtx(NULL)
{
#ifdef _DEBUG
  setDebugName("FCDialog");
#endif//_DEBUG

	IGUISkin* skin = Environment->getSkin();
	IGUISpriteBank* sprites = 0;
	video::SColor color(255,255,255,255);
	if (skin)
	{
		sprites = skin->getSpriteBank();
		color = skin->getColor(EGDC_WINDOW_SYMBOL);
	}

	s32 buttonw = Environment->getSkin()->getSize(EGDS_WINDOW_BUTTON_WIDTH);
	s32 posx = RelativeRect.getWidth() - buttonw - 4;

	m_CloseButton = Environment->addButton(core::rect<s32>(posx, 3, posx + buttonw, 3 + buttonw), this, -1,
		L"", skin ? skin->getDefaultText(EGDT_WINDOW_CLOSE) : L"Close");
	m_CloseButton->setSubElement(true);
	m_CloseButton->setTabStop(false);
	if (sprites)
	{
		m_CloseButton->setSpriteBank(sprites);
		m_CloseButton->setSprite(EGBS_BUTTON_UP, skin->getIcon(EGDI_WINDOW_CLOSE), color);
		m_CloseButton->setSprite(EGBS_BUTTON_DOWN, skin->getIcon(EGDI_WINDOW_CLOSE), color);
	}
	m_CloseButton->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
	m_CloseButton->grab();

	Text = caption;

  Environment->setFocus(this);
}

////////////////////////////////////////////////////////////////////////////////

FCDialog::~FCDialog(void)
{
	if (m_CloseButton)
		m_CloseButton->drop();
}

////////////////////////////////////////////////////////////////////////////////

void FCDialog::CenterWindow()
{
  core::dimension2d<s32> screenDim = Environment->getVideoDriver()->getScreenSize();
  core::rect<s32> wndRect = AbsoluteRect;

  setRelativePosition( position2di( screenDim.Width/2 - wndRect.getWidth()/2, screenDim.Height/2 - wndRect.getHeight()/2) );
}

////////////////////////////////////////////////////////////////////////////////

void FCDialog::draw()
{
	if (!IsVisible)
		return;

	IGUISkin* skin = Environment->getSkin();

	core::rect<s32> rect = AbsoluteRect;
	core::rect<s32> *cl = &AbsoluteClippingRect;

	// draw body fast
	rect = skin->draw3DWindowBackground(this, true, skin->getColor(EGDC_ACTIVE_BORDER),
		AbsoluteRect, &AbsoluteClippingRect);

	if (Text.size())
	{
		rect.UpperLeftCorner.X += skin->getSize(EGDS_TEXT_DISTANCE_X);
		rect.UpperLeftCorner.Y += skin->getSize(EGDS_TEXT_DISTANCE_Y);
		rect.LowerRightCorner.X -= skin->getSize(EGDS_WINDOW_BUTTON_WIDTH) + 5;

		IGUIFont* font = skin->getFont(EGDF_WINDOW);
		if (font)
			font->draw(Text.c_str(), rect, skin->getColor(EGDC_ACTIVE_CAPTION), false, true, cl);
	}

	IGUIElement::draw();
}

////////////////////////////////////////////////////////////////////////////////

bool FCDialog::OnEvent(const SEvent& event)
{
	if (IsEnabled)
	{
		switch(event.EventType)
		{
		case EET_GUI_EVENT:
			{
				switch(event.GUIEvent.EventType)
				{
				case EGET_ELEMENT_FOCUSED:
					// only children are allowed the focus
					if (event.GUIEvent.Caller != this && !isMyChild(event.GUIEvent.Caller))
						Environment->setFocus(this);
					return false;
				case EGET_ELEMENT_FOCUS_LOST:
					// only children are allowed the focus
					if (!(isMyChild(event.GUIEvent.Element) || event.GUIEvent.Element == this))
					{
						return true;
					}
					else
					{
						return IGUIElement::OnEvent(event);
					}
					break;

				case	EGET_CHECKBOX_CHANGED:
					{
						if ( isMyChild(event.GUIEvent.Caller) )
						{
							IGUICheckBox* pCB = (IGUICheckBox*) event.GUIEvent.Caller;
							return OnCheckboxChanged(event.GUIEvent.Caller->getID(), pCB);
						}
					}
					break;

				case	EGET_COMBO_BOX_CHANGED:
					{
						if ( isMyChild(event.GUIEvent.Caller) )
						{
							IGUIComboBox* pCB = (IGUIComboBox*) event.GUIEvent.Caller;
							return OnComboBoxChanged(event.GUIEvent.Caller->getID(), pCB);
						}
					}
					break;

				case	EGET_EDITBOX_ENTER:
					{
						if ( isMyChild(event.GUIEvent.Caller) )
						{
							IGUIEditBox* pEdit = (IGUIEditBox*) event.GUIEvent.Caller;
							return OnEditBoxEnter(event.GUIEvent.Caller->getID(), pEdit);
						}
					}
					break;

				case	EGET_LISTBOX_CHANGED:
					{
						if ( isMyChild(event.GUIEvent.Caller) )
						{
							IGUIListBox* pLB = (IGUIListBox*) event.GUIEvent.Caller;
							return OnListBoxChanged(event.GUIEvent.Caller->getID(), pLB);
						}
					}
					break;

				case	EGET_LISTBOX_SELECTED_AGAIN:
					{
						if ( isMyChild(event.GUIEvent.Caller) )
						{
							IGUIListBox* pLB = (IGUIListBox*) event.GUIEvent.Caller;
							return OnListBoxSelectedAgain(event.GUIEvent.Caller->getID(), pLB);
						}
					}
					break;

				case	EGET_SCROLL_BAR_CHANGED:
					{
						if ( isMyChild(event.GUIEvent.Caller) )
						{
							IGUIScrollBar* pSB = (IGUIScrollBar*) event.GUIEvent.Caller;
							return OnScrollBarChanged(event.GUIEvent.Caller->getID(), pSB);
						}
					}
					break;

				case	EGET_SPINBOX_CHANGED:
					{
						if ( isMyChild(event.GUIEvent.Caller) )
						{
							IGUISpinBox* pSB = (IGUISpinBox*) event.GUIEvent.Caller;
							return OnSpinBoxChanged(event.GUIEvent.Caller->getID(), pSB);
						}
					}
					break;

				case EGET_BUTTON_CLICKED:
					{
						// check if the window close button was clicked
						if ( isMyChild(event.GUIEvent.Caller) ) 
						{
							if (event.GUIEvent.Caller == m_CloseButton || event.GUIEvent.Caller->getID() == FCBTN_CANCEL )
							{
                OnCancel();
								return true;
							}
              else if ( event.GUIEvent.Caller->getID() == FCBTN_OK )
              {
                OnOK();
                return true;
              }

							// if not... lets pass the event on if its one of our children
							IGUIButton* pBtn = (IGUIButton*) event.GUIEvent.Caller;
							return OnButtonClicked(event.GUIEvent.Caller->getID(), pBtn);
						}
					}
					break;

				case EGET_ELEMENT_CLOSED:
					// do not interfere with children being removed
					return IGUIElement::OnEvent(event);

        case EGET_MENU_ITEM_SELECTED:
          {
            IGUIContextMenu* pMenu = (IGUIContextMenu*)event.GUIEvent.Caller;
            s32 selItem = pMenu->getSelectedItem();
            return OnMenuItemSelected(selItem, pMenu);
          }
          break;

				default:
          {
            if ( isMyChild(event.GUIEvent.Caller) )
            {
              OnUnknownGUIEvent(event.GUIEvent.EventType, event.GUIEvent.Caller->getID(), event.GUIEvent.Element);
            }
          }
					break;
				}
			}
			break;

		case EET_MOUSE_INPUT_EVENT:
			{
				switch(event.MouseInput.Event)
				{
				case EMIE_MOUSE_WHEEL:
					break;

				case EMIE_LMOUSE_PRESSED_DOWN:
					if ( isPointInside( position2d<s32>( event.MouseInput.X, event.MouseInput.Y ) ) )
					{
						DragStart.X = event.MouseInput.X;
						DragStart.Y = event.MouseInput.Y;
						Dragging = true;
					}
					Environment->setFocus(this);
					return true;

				case EMIE_LMOUSE_LEFT_UP:
					Dragging = false;
					return true;

				case EMIE_MOUSE_MOVED:
					if (Dragging)
					{
						// gui window should not be dragged outside its parent
						if (Parent)
						{
							if (event.MouseInput.X < Parent->getAbsolutePosition().UpperLeftCorner.X +1 ||
								event.MouseInput.Y < Parent->getAbsolutePosition().UpperLeftCorner.Y +1 ||
								event.MouseInput.X > Parent->getAbsolutePosition().LowerRightCorner.X -1 ||
								event.MouseInput.Y > Parent->getAbsolutePosition().LowerRightCorner.Y -1)
							{
								return true;
							}
						}

						move(core::position2d<s32>(event.MouseInput.X - DragStart.X, event.MouseInput.Y - DragStart.Y));
						DragStart.X = event.MouseInput.X;
						DragStart.Y = event.MouseInput.Y;
						return true;
					}
					break;

				default:
					if ( IsVisible )
						return true;
					break;
				}
			}
			break;

		default:
			break;
		}
	}

	return IGUIElement::OnEvent(event);

}

////////////////////////////////////////////////////////////////////////////////

void FCDialog::OnOK()
{
  if ( m_fpSuccessCallback )
    (*m_fpSuccessCallback)(m_pSuccessCtx);
  closeDialog();
  delete this;
}

////////////////////////////////////////////////////////////////////////////////

void FCDialog::OnCancel()
{
  if ( m_fpCancelCallback )
	  (*m_fpCancelCallback)(m_pCancelCtx);
  closeDialog();
  delete this;
}

////////////////////////////////////////////////////////////////////////////////

void FCDialog::closeDialog()
{
  if ( Parent )
  {
		// send close event to parent
		SEvent e;
		e.EventType = EET_GUI_EVENT;
		e.GUIEvent.Caller = this;
		e.GUIEvent.Element = 0;
		e.GUIEvent.EventType = EGET_ELEMENT_CLOSED;

		// if the event was not absorbed
		if (!Parent->OnEvent(e))
			remove();  
  }
  else
	  remove();
}