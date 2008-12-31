#include "FCController.h"
#include "FCView.h"

FCView::FCView(void)
: m_pModel(NULL)
, m_pController(NULL)
, m_pDevice(NULL)
, m_pDriver(NULL)
, m_pEnv(NULL)
{
}

///////////////////////////////////////////////////////////////////////

FCView::~FCView(void)
{
	if ( m_pDevice )
		m_pDevice->drop();
}

///////////////////////////////////////////////////////////////////////

void FCView::SetModel(FCModel* pModel)
{
	m_pModel = pModel;
	pModel->SubscribeToEvents(this);
}

///////////////////////////////////////////////////////////////////////

void FCView::SetController(FCController* pController)
{
	m_pController = pController;
}

///////////////////////////////////////////////////////////////////////

bool FCView::Initialise(E_DRIVER_TYPE driverType)
{
	IrrlichtDevice* pDev = createDevice(driverType);
	IVideoDriver* pDriver = NULL;
	s32 vDepth = 16;
	dimension2d<s32> vRes(800, 600);

	if ( pDev )
	{
		IVideoModeList* pModeList = pDev->getVideoModeList();

		if ( pModeList )
		{
			vDepth = pModeList->getDesktopDepth();
			vRes = pModeList->getDesktopResolution();
		}
	}

	pDev->drop();

	// now create the real device
	m_pDevice = createDevice(driverType, vRes, vDepth, true);
	if ( !m_pDevice )
		return false;

	m_pDriver = m_pDevice->getVideoDriver();
	if ( !m_pDriver )
		return false;

	m_pEnv = m_pDevice->getGUIEnvironment();
	if ( !m_pEnv )
		return false;

	// load the font
	m_pEnv->getFont("./clientdata/fonts/fontcourier.bmp");

	return true;
}