#include "FCController.h"
#include "FCView.h"

FCView::FCView(void)
: m_pModel(NULL)
, m_pController(NULL)
, m_pDevice(NULL)
, m_pDriver(NULL)
, m_pEnv(NULL)
, m_pScene(NULL)
, m_currentModelState(FCModel::None)
, m_pCurrentViewLogic(NULL)
{
}

///////////////////////////////////////////////////////////////////////

FCView::~FCView(void)
{
	if ( m_pCurrentViewLogic )
		m_pCurrentViewLogic->Destroy();

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
	IrrlichtDevice* pDev = createDevice(EDT_NULL);
	IVideoDriver* pDriver = NULL;
	s32 vDepth = 16;
	dimension2d<s32> vRes(800, 600);

	// get the current desktop resolution and bit depth from the NULL device
	if ( pDev )
	{
		IVideoModeList* pModeList = pDev->getVideoModeList();

		if ( pModeList )
		{
			vDepth = pModeList->getDesktopDepth();
			vRes = pModeList->getDesktopResolution();
		}
	}

	// release the NULL device since we no longer need it
	pDev->closeDevice();
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

	m_pScene = m_pDevice->getSceneManager();
	if ( !m_pScene )
		return false;

	// load the font
	m_pEnv->getFont("./clientdata/fonts/fontcourier.bmp");

	// Create event queue thread
	QueueReaderThread<FCView, FCModelEvent>::ThreadContext* pCtx = new QueueReaderThread<FCView, FCModelEvent>::ThreadContext;
	pCtx->pCallbackObject = this;
	pCtx->pSourceQueue = &m_modelEventQueue;
	pCtx->pCallback = &FCView::HandleEvent;
	m_thrdEventQueue.Start( (long long)pCtx );

	return true;
}

///////////////////////////////////////////////////////////////////////

bool FCView::Update()
{
	bool bResult = true;
	SColor colBkg = SColor(255, 0, 0, 0);

	if ( m_pDevice->run() && m_pDriver )
	{
		if ( m_pCurrentViewLogic )
			colBkg = m_pCurrentViewLogic->GetBackgroundColour();
		m_pDriver->beginScene(true, true, colBkg);
		if ( m_pCurrentViewLogic )
			m_pCurrentViewLogic->Refresh();
		m_pDriver->endScene();
	}
	else
		bResult = false;

	return bResult;
}

///////////////////////////////////////////////////////////////////////

void FCView::OnModelEvent(FCModelEvent event)
{
	m_modelEventQueue.push_back(event);
}

///////////////////////////////////////////////////////////////////////

void FCView::HandleEvent(FCModelEvent& e)
{
	switch ( e.GetType() )
	{
	case	FCME_StateChange:
		{
			FCModel::e_ModelState newState = static_cast<FCModel::e_ModelState>(e.GetData());

			if ( newState != m_currentModelState )
			{
				IViewLogic* pNewView = NULL;

				switch ( newState )
				{
				case	FCModel::Loading:
					pNewView = &m_vlLoading;
					break;

				case	FCModel::Login:
					break;

				case	FCModel::Playing:
					break;

				case	FCModel::ShuttingDown:
					break;
				}

				if ( pNewView )
				{
					pNewView->Create(m_pDevice);
					if ( m_pCurrentViewLogic )
						m_pCurrentViewLogic->Destroy();
					m_pCurrentViewLogic = pNewView;
					m_pCurrentViewLogic->SetActive();
				}
			}
		}
		break;

	default:
		break;
	}
}
