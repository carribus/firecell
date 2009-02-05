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
#include "Country.h"
#include "DlgNewCharacter.h"
#include "InGameAppWindow.h"
#include "ViewLogicCharacterSelection.h"

#define STATIC_HEADER               1
#define BUTTON_NEWCHAR							2
#define DIALOG_NEWCHAR							50
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
, m_pActiveDialog(NULL)
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

  switch ( event.GetType() )
  {
  case  FCME_StateChange:
    {
      FCModel::StateInfo state = m_pModel->GetState();

      if ( state.state == FCModel::CharacterSelection )
      {
        switch ( state.stateStep )
        {
        case  FCModel::MS_CharacterSelection_CharacterSelected:
          {
            core::dimension2d<s32> screenSize = m_pDevice->getVideoDriver()->getScreenSize();
            core::rect<s32> tRect( 0, 0, screenSize.Width, screenSize.Height );
            IGUIStaticText* pText = m_pEnv->addStaticText( ResourceManager::instance().GetClientString(STR_CHARSEL_LOADINGCHAR).c_str(), tRect, false, false );
            pText->setTextAlignment( EGUIA_CENTER, EGUIA_CENTER );
            pText->setOverrideColor( SColor(255, 255, 0, 0) );
          }
          break;

        default:
          break;
        }
      }
    }
    break;

  case  FCME_NewCharacterParamsReady:
    {
      FCModel::StateInfo state = m_pModel->GetState();

      // check that we are in character selection phase, and that the model is ready for new character creation
      if ( state.state == FCModel::CharacterSelection && state.stateStep == FCModel::MS_CharacterSelection_NewCharacter )
      {
        // check if we have an active dialog already showing (we shouldn't at this point)
        if ( !m_pActiveDialog )
        {
          std::map<FCULONG, Country>& mapCountries = m_pModel->GetCountries();

          ShowNewCharacterForm();

          ((DlgNewCharacter*)m_pActiveDialog)->populateGeographyControls(mapCountries);
        }
      }
    }
    break;

  default:
    break;
  }

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
			case	EGET_BUTTON_CLICKED:
				{
					switch ( elemID )
					{
					case	BUTTON_NEWCHAR:
            {
              // the user has chosen to create a new character...
              // Notify the model of the request
						  FCViewEvent e( VE_NewCharacterRequested );
						  m_pContainer->GetController()->OnViewEvent(e);

              bHandled = true;
            }
						break;

					default:
						break;
					}
				}
				break;
      }
    }
		break;

  default:
    break;
  }

	return bHandled;
}

///////////////////////////////////////////////////////////////////////

void ViewLogicCharacterSelection::OnDlgNewCharacterCancelled(void* pCtx)
{
  ViewLogicCharacterSelection* pThis = (ViewLogicCharacterSelection*)pCtx;
  pThis->m_pActiveDialog = NULL;
}

///////////////////////////////////////////////////////////////////////

void ViewLogicCharacterSelection::OnDlgNewCharacterCompleted(void* pCtx)
{
  ViewLogicCharacterSelection* pThis = (ViewLogicCharacterSelection*)pCtx;
  pThis->m_pActiveDialog = NULL;
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

    // pass the container to the gui object
    pItem->setContainer( m_pContainer );

    // position the gui object
    pItem->setRelativePosition( posItem );

    // offset the position
    posItem.Y += pItem->getAbsoluteClippingRect().getHeight() + CHARSEL_ITEM_PADDING_TOP;
    pItem->drop();
  }

	// max number of characters per player is 8 at the moment...
	// TODO: Make this a configuration in the database
	if ( numChars < 8 )
	{
		core::dimension2d<s32> dim = m_pDevice->getVideoDriver()->getScreenSize();
		core::rect<s32> btnRect( dim.Width - BORDER_EDGE_RIGHT - 200,
														 dim.Height - BORDER_EDGE_BOTTOM - 50,
                             dim.Width - BORDER_EDGE_RIGHT - 10,
                             dim.Height - BORDER_EDGE_BOTTOM - 10 );
		// create the "New Character" option
		m_pEnv->addButton( btnRect, 0, BUTTON_NEWCHAR, ResourceManager::instance().GetClientString( STR_CHARSEL_NEWCHAR ).c_str() );
	}
}

///////////////////////////////////////////////////////////////////////

void ViewLogicCharacterSelection::ShowNewCharacterForm()
{
  if ( m_pActiveDialog )
    return;

	DlgNewCharacter* pDlg = new DlgNewCharacter(m_pEnv, (wchar_t*)ResourceManager::instance().GetClientString(STR_CHARSEL_NEWCHAR_WNDCAPTION).c_str(), 0, DIALOG_NEWCHAR);

  if (pDlg)
  {
    // set the callbacks
	  pDlg->setCancelCallback(OnDlgNewCharacterCancelled, (void*)this);
	  pDlg->setSuccessCallback(OnDlgNewCharacterCompleted, (void*)this);

    // set the active dialog
    m_pActiveDialog = pDlg;
  }
}

///////////////////////////////////////////////////////////////////////
