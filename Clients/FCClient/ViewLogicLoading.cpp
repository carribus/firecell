#include "ViewLogicLoading.h"

ViewLogicLoading::ViewLogicLoading(void)
: m_pDevice(NULL)
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

void ViewLogicLoading::Create(IrrlichtDevice* pDevice)
{
	m_lineIndex = 0;
	m_arrDetailLines.push_back( L"Runtime check #1\n" );
	m_arrDetailLines.push_back( L"Runtime check #2\n" );
	m_arrDetailLines.push_back( L"Runtime check #3\n" );
	m_arrDetailLines.push_back( L"Runtime check #4\n" );

	// get the required interfaces and store them
	m_pDevice = pDevice;
	m_pScene = m_pDevice->getSceneManager();
	m_pEnv = m_pDevice->getGUIEnvironment();
	core::dimension2d<s32> dim = m_pDevice->getVideoDriver()->getScreenSize();

	// setup the font
	IGUIFont* pFont = m_pEnv->getFont("./clientdata/fonts/fontcourier.bmp");

	// create the 'loading' text object
	m_strDetails += m_arrDetailLines[ m_lineIndex ];
	m_pTextObject = m_pEnv->addStaticText(m_strDetails.c_str(), core::rect<s32>(10, 10, dim.Width, dim.Height), false);

	m_pTextObject->setOverrideColor( SColor(255, 172, 172, 172) );
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
	static u32 lastTick = 0;
	u32 now = m_pDevice->getTimer()->getTime();

	if ( now - lastTick > 2500 && m_lineIndex < m_arrDetailLines.size()-1 )
	{
		m_strDetails += m_arrDetailLines.at( ++m_lineIndex );
		m_pTextObject->setText(m_strDetails.c_str());
		lastTick = now;
	}

	m_pScene->drawAll();
	m_pEnv->drawAll();
}
