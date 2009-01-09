#include "ViewLogicLoading.h"

ViewLogicLoading::ViewLogicLoading()
: m_pContainer(NULL)
, m_pDevice(NULL)
, m_pScene(NULL)
, m_pEnv(NULL)
, m_pTextObject(NULL)
{
}

///////////////////////////////////////////////////////////////////////

ViewLogicLoading::~ViewLogicLoading(void)
{
}

///////////////////////////////////////////////////////////////////////

void ViewLogicLoading::Create(FCView* pContainer, IrrlichtDevice* pDevice)
{
  m_arrDetailLines.push_back( L"FireCell initializing...\n");   // index 0
	m_arrDetailLines.push_back( L"Loading Textual data\n" );
	m_arrDetailLines.push_back( L"Loading Graphical assets\n" );
	m_arrDetailLines.push_back( L"Loading Audio assets\n" );
	m_arrDetailLines.push_back( L"------------------------\n" );
	m_arrDetailLines.push_back( L"Connecting to server... " );
	m_arrDetailLines.push_back( L"OK\n");
	m_arrDetailLines.push_back( L"Failed\nRetrying...\n" );
	m_arrDetailLines.push_back( L"Failed\n");

	m_pContainer = pContainer;

	// get the required interfaces and store them
	m_pDevice = pDevice;
	m_pScene = m_pDevice->getSceneManager();
	m_pEnv = m_pDevice->getGUIEnvironment();
	core::dimension2d<s32> dim = m_pDevice->getVideoDriver()->getScreenSize();

	// setup the font
	IGUIFont* pFont = m_pEnv->getFont("./clientdata/fonts/fontfixedsys.xml");

	// create the 'loading' text object
	m_strDetails += m_arrDetailLines[ 0 ];
	m_pTextObject = m_pEnv->addStaticText(m_strDetails.c_str(), core::rect<s32>(10, 10, dim.Width, dim.Height), false);

	m_pTextObject->setOverrideColor( SColor(255, 0, 255, 0) );
	m_pTextObject->setOverrideFont(pFont);
}

///////////////////////////////////////////////////////////////////////

void ViewLogicLoading::Destroy()
{
	m_pEnv->clear();
	m_pScene->clear();
}

///////////////////////////////////////////////////////////////////////

void ViewLogicLoading::SetActive()
{
}

///////////////////////////////////////////////////////////////////////

void ViewLogicLoading::Refresh()
{
	m_pScene->drawAll();
	m_pEnv->drawAll();
}

///////////////////////////////////////////////////////////////////////

void ViewLogicLoading::OnModelStateChange(FCModel::StateInfo state)
{
  if ( state.state != FCModel::Loading && state.state != FCModel::Connecting )
    return;

  // check the Model's current sub-state and take necessary action
  switch ( state.state )
  {
  case  FCModel::Loading:
    {
      switch ( state.stateStep )
      {
      case  FCModel::MS_Loading_Text:
        m_strDetails += m_arrDetailLines.at( state.stateStep );
        break;

      case  FCModel::MS_Loading_Graphics:
        m_strDetails += m_arrDetailLines.at( state.stateStep );
        break;

      case  FCModel::MS_Loading_Sounds:
        m_strDetails += m_arrDetailLines.at( state.stateStep );
        break;
      }

      m_pTextObject->setText(m_strDetails.c_str());
    }
    break;

  case  FCModel::Connecting:
		{
			switch ( state.stateStep )
			{
			case	FCModel::MS_Connecting_None:
				m_strDetails += m_arrDetailLines.at( state.stateStep + 4 );
				break;

			case	FCModel::MS_Connecting_Connecting:
				m_strDetails += m_arrDetailLines.at( state.stateStep + 4 );
				break;

			case	FCModel::MS_Connecting_Retry:
				m_strDetails += m_arrDetailLines.at( state.stateStep + 4 );
				break;

			case	FCModel::MS_Connecting_Connected:
				m_strDetails += m_arrDetailLines.at( state.stateStep + 4 );
				break;

			case	FCModel::MS_Connecting_FinalFail:
				m_strDetails += m_arrDetailLines.at( state.stateStep + 4 );
				break;
			}

			m_pTextObject->setText(m_strDetails.c_str());
		}
    break;

  default:
    break;
  }
}