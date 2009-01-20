/*
    FireCell Server - The server code for the firecell multiplayer game
    Copyright (C) 2008  Peter M. Mares

		Contact: carribus@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "../common/ResourceManager.h"
#include "clientstrings.h"
#include "FCController.h"
#include "FCView.h"
#include "FCViewEvent.h"
#include "ViewLogicCharacterSelection.h"

#define STATIC_HEADER               1
#define CHARSEL_OBJECT_BASE         100

#define BORDER_EDGE_TOP             30
#define BORDER_EDGE_LEFT            20
#define BORDER_EDGE_RIGHT           20
#define BORDER_EDGE_BOTTOM          20

#define CHARSEL_ITEM_STARTX         50
#define CHARSEL_ITEM_STARTY         75
#define CHARSEL_ITEM_PADDING_TOP    10

ViewLogicCharacterSelection::ViewLogicCharacterSelection(void)
: m_pModel(NULL)
, m_pContainer(NULL)
, m_pDevice(NULL)
, m_pScene(NULL)
, m_pEnv(NULL)
{
}

///////////////////////////////////////////////////////////////////////

ViewLogicCharacterSelection::~ViewLogicCharacterSelection(void)
{
}

///////////////////////////////////////////////////////////////////////

void ViewLogicCharacterSelection::Create(FCView* pContainer, IrrlichtDevice* pDevice)
{
	m_pContainer = pContainer;
  m_pModel = m_pContainer->GetModel();

	// get the required interfaces and store them
	m_pDevice = pDevice;
	m_pScene = m_pDevice->getSceneManager();
	m_pEnv = m_pDevice->getGUIEnvironment();

	// setup the event receiver
	pDevice->setEventReceiver(this);

	// setup the font
	m_pNormalFont = m_pEnv->getFont("./clientdata/fonts/fontcourier.png");
	m_pEnv->getSkin()->setFont(m_pNormalFont);

	if ( !(m_pFontHeader = m_pEnv->getFont("./clientdata/fonts/fontverdana_18px.xml")) )
    m_pFontHeader = m_pNormalFont;
}

///////////////////////////////////////////////////////////////////////

void ViewLogicCharacterSelection::Destroy()
{
	m_pEnv->clear();
	m_pScene->clear();
}

///////////////////////////////////////////////////////////////////////

void ViewLogicCharacterSelection::SetActive()
{
	CreateGUIObjects();
}

///////////////////////////////////////////////////////////////////////

void ViewLogicCharacterSelection::Refresh()
{
	core::dimension2d<s32> dim = m_pDevice->getVideoDriver()->getScreenSize();
	core::rect<s32> frame(BORDER_EDGE_LEFT, BORDER_EDGE_TOP, dim.Width - BORDER_EDGE_RIGHT, dim.Height - BORDER_EDGE_BOTTOM);
	IVideoDriver* pDriver = m_pDevice->getVideoDriver();

	pDriver->draw2DRectangle(frame, 
													 SColor(255, 64, 64, 64),
													 SColor(255, 64, 64, 64),
													 SColor(255, 0, 0, 0),
													 SColor(255, 0, 0, 0) );

  m_pScene->drawAll();
	m_pEnv->drawAll();
}

///////////////////////////////////////////////////////////////////////

bool ViewLogicCharacterSelection::OnModelEvent(FCModelEvent event)
{
  bool bResult = false;

  return bResult;
}

///////////////////////////////////////////////////////////////////////

bool ViewLogicCharacterSelection::OnEvent(const SEvent& event)
{
	bool bHandled = false;

  switch ( event.EventType )
  {
  case  EET_GUI_EVENT :
    {
      s32 elemID = event.GUIEvent.Caller->getID();
      IGUIElement* pRoot = m_pEnv->getRootGUIElement();
      IGUIElement* pElem = event.GUIEvent.Caller;
      core::list<IGUIElement*> children = pRoot->getChildren();

      switch ( event.GUIEvent.EventType )
      {
      case  EGET_ELEMENT_HOVERED:
        {
          if ( elemID >= 100 )
          {
            GUIFCCharacterItem* pItem = (GUIFCCharacterItem*) pElem;

            if ( pItem )
            {
              pItem->SetHighlight(true);
            }
          }
        }
        break;

      case  EGET_ELEMENT_LEFT:
        {
          if ( elemID >= 100 )
          {
            GUIFCCharacterItem* pItem = (GUIFCCharacterItem*) pElem;

            if ( pItem )
            {
              pItem->SetHighlight(false);
            }
          }
        }
        break;
      }
    }
		break;

	case	EET_MOUSE_INPUT_EVENT:
		{
			switch ( event.MouseInput.Event )
			{
			case	EMIE_LMOUSE_LEFT_UP:
				{
					IGUIElement* pElem = m_pEnv->getRootGUIElement()->getElementFromPoint( core::position2d<s32>( event.MouseInput.X, event.MouseInput.Y ) );
					
					if ( pElem )
					{
						if ( pElem->getID() >= 100 )
						{
							GUIFCCharacterItem* pItem = (GUIFCCharacterItem*) pElem;

							Character* pChar = pItem->GetCharacter();

							if ( pChar )
							{
								FCViewEvent e( VE_CharacterSelected, pChar->GetID() );
								m_pContainer->GetController()->OnViewEvent(e);
							}
						}
					}
				}
				break;

			default:
				break;
			}
		}
		break;


  }

	return bHandled;
}

///////////////////////////////////////////////////////////////////////

void ViewLogicCharacterSelection::CreateGUIObjects()
{
  vector<Character>& characters = m_pModel->GetCharacters();
  size_t numChars = characters.size();
  core::position2di posItem(CHARSEL_ITEM_STARTX, CHARSEL_ITEM_STARTY);

  // setup the header
  wstring strHeader = ResourceManager::instance().GetClientString( STR_CHARSEL_HEADER );
  core::dimension2d<s32> txtDim = m_pFontHeader->getDimension( strHeader.c_str() );
  core::dimension2d<s32> screenDim = m_pDevice->getVideoDriver()->getScreenSize();
  IGUIStaticText* pHeader = m_pEnv->addStaticText( strHeader.c_str(), core::rect<s32>(0, 0, screenDim.Width, txtDim.Height), false, false, 0, STATIC_HEADER );
  pHeader->setOverrideFont( m_pFontHeader );
  pHeader->setTextAlignment( EGUIA_CENTER, EGUIA_CENTER );
  pHeader->setOverrideColor( video::SColor(255, 196, 196, 196) );

  for ( size_t i = 0; i < numChars; i++ )
  {
    GUIFCCharacterItem* pItem = new GUIFCCharacterItem( m_pEnv, m_pEnv->getRootGUIElement(), CHARSEL_OBJECT_BASE + (s32)i );

    // get the character and assign it to the gui reference
    Character& c = characters[i];
    pItem->SetCharacter(&c);

    // position the gui object
    pItem->setRelativePosition( posItem );

    // offset the position
    posItem.Y += pItem->getAbsoluteClippingRect().getHeight() + CHARSEL_ITEM_PADDING_TOP;
  }
}

///////////////////////////////////////////////////////////////////////

void ViewLogicCharacterSelection::OnModelStateChange(FCModel::StateInfo state)
{
}

