#include <map>
#include "../common/clienttypes.h"
#include "FCController.h"
#include "FCViewEvent.h"
#include "ForumWindow.h"

ForumWindow::ForumWindow(IDesktop* pDesktop, FCController* pController, IrrlichtDevice* pDevice)
: InGameAppWindow(pDesktop, pController, pDevice->getGUIEnvironment())
, m_pDevice(pDevice)
, m_pForumCatBrowser(NULL)
, m_pForumThreadBrowser(NULL)
, m_pModel(NULL)
{
}

///////////////////////////////////////////////////////////////////////

ForumWindow::~ForumWindow(void)
{
}

///////////////////////////////////////////////////////////////////////

bool ForumWindow::Create(s32 AppElemID, FCUINT optionID, std::wstring caption)
{
  bool bResult = InGameAppWindow::Create(AppElemID, optionID, DOT_Forum, caption);
	core::rect<s32> wndRect, clientRect;

  // calculate the size of the window
  m_pDesktop->GetDesktopRect(wndRect);
  wndRect.UpperLeftCorner.X += wndRect.getWidth()/4;
  wndRect.UpperLeftCorner.Y += wndRect.getHeight()/4;

  // update the window
	if ( bResult )
	{
		m_pWindow->setRelativePosition(wndRect);
    CenterWindow();
		GetClientRect(clientRect);
  }

  // create the forum browsing component
	GetClientRect(clientRect);
  m_pForumCatBrowser = new GUIForumCatBrowser(m_pEnv, clientRect, m_pWindow);
	m_pForumCatBrowser->registerSink(this);
  m_pForumCatBrowser->setVisible(true);
  m_pForumCatBrowser->drop();

  // create the thread browsing component
  m_pForumThreadBrowser = new GUIForumThreadBrowser(m_pEnv, clientRect, m_pWindow);
  m_pForumThreadBrowser->registerSink(this);
  m_pForumThreadBrowser->setVisible(false);
  m_pForumThreadBrowser->drop();

  m_pEnv->setFocus( m_pForumCatBrowser );

  RequestForumCategoryRefresh();

  return bResult;
}

///////////////////////////////////////////////////////////////////////

bool ForumWindow::OnCategoriesReceived(ForumModel* pModel)
{
	if ( !pModel )
		return false;

	// if we don't have a reference to the model yet, then set it now
	if ( !m_pModel )
		m_pModel = pModel;

	m_pForumCatBrowser->setModel(pModel);
	m_pForumCatBrowser->updateCategories();
  SetWaitingForResponse(false);

	return true;
}

///////////////////////////////////////////////////////////////////////

bool ForumWindow::OnCategoryThreadsReceived(FCULONG category_id)
{
  if ( !m_pModel )
    return false;

  m_pForumThreadBrowser->setModel(m_pModel);
  m_pForumThreadBrowser->updateCategoryThreads(category_id);
  SetWaitingForResponse(false);

  return true;
}

///////////////////////////////////////////////////////////////////////

bool ForumWindow::OnThreadContentReceived(ForumThread* pThread)
{
  if ( !pThread )
    return false;

  // TODO: Update the forum thread reader
  SetWaitingForResponse(false);

  return true;
}

///////////////////////////////////////////////////////////////////////

void ForumWindow::OnCategorySelected(FCULONG category_id)
{
  ForumCategory* pCat = m_pModel->getCategoryByID(category_id);
  if ( pCat )
  {
    m_pForumCatBrowser->setVisible(false);
    m_pForumThreadBrowser->setCategory(pCat);
    m_pForumThreadBrowser->clearThreads();
    m_pForumThreadBrowser->setVisible(true);
    m_pEnv->setFocus( m_pForumThreadBrowser );

    // request the threads for the category from the server
	  SetWaitingForResponse(true);
	  FCViewEvent e(VE_ForumCategorySelected, (long long)category_id);
	  m_pController->OnViewEvent(e);
  }
}

///////////////////////////////////////////////////////////////////////

void ForumWindow::OnThreadSelected(FCULONG category_id, FCULONG thread_id)
{
  // hide the thread browser and show the thread details view
//  m_pForumThreadBrowser->setVisible(false);

  // request the selected thread's details
  SetWaitingForResponse(true);
  FCViewEventThreadSelected e(category_id, thread_id);
  m_pController->OnViewEvent(e);
}

///////////////////////////////////////////////////////////////////////

void ForumWindow::OnThreadViewClosed()
{
  m_pForumThreadBrowser->setVisible(false);
  m_pForumCatBrowser->setVisible(true);
  m_pEnv->setFocus(m_pForumCatBrowser);
}

///////////////////////////////////////////////////////////////////////

void ForumWindow::OnNewThreadPost(FCULONG category_id, std::wstring& subject, std::wstring& message)
{
  SetWaitingForResponse(true);
  FCViewEventNewForumPost e(category_id, subject, message);
  m_pController->OnViewEvent(e);
}

///////////////////////////////////////////////////////////////////////

void ForumWindow::RequestForumCategoryRefresh()
{
  FCViewEvent e(VE_ForumRefreshCategories);
  
  SetWaitingForResponse(true);
  m_pController->OnViewEvent(e);
}