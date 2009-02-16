#ifndef _FORUMWINDOW_H_
#define _FORUMWINDOW_H_

#include <string>
#include "FCModel.h"
#include "ForumModel.h"
#include "InGameAppWindow.h"
#include "GUIForumCatBrowser.h"
#include "GUIForumThreadBrowser.h"

class ForumWindow : public InGameAppWindow
									, public IForumCatBrowserSink
                  , public IForumThreadBrowserSink
{
public:
  ForumWindow(IDesktop* pDesktop, FCController* pController, IrrlichtDevice* pDevice);
  ~ForumWindow(void);

  bool Create(s32 AppElemID, FCUINT optionID, std::wstring caption);

	/**
	 *	@brief Event handler for when a category update has been received from the server
	 */
	bool OnCategoriesReceived(ForumModel* pModel);

  /**
   *  @brief Event handler for when threads for a specific category have been received from the server
   */
  bool OnCategoryThreadsReceived(FCULONG category_id);

  /**
   *  @brief Event handler for when the content for a particular thread has been received from the server
   */
  bool OnThreadContentReceived(ForumThread* pThread);

	/*
	 *	IForumCatBrowserSink implementation
	 */
	void OnCategorySelected(FCULONG category_id);

  /*
   *  IForumThreadBrowserSink implementation
   */
  void OnThreadSelected(FCULONG category_id, FCULONG thread_id);
  void OnThreadViewClosed();
  void OnNewThreadPost(FCULONG category_id, std::wstring& subject, std::wstring& message);

  const wchar_t* getAppName()                             { return L"Forum Browser"; }

private:

  /*
   *  Private Methods
   */
  void RequestForumCategoryRefresh();

  /*
   *  Private Members
   */
  IrrlichtDevice*         m_pDevice;
  GUIForumCatBrowser*     m_pForumCatBrowser;
  GUIForumThreadBrowser*  m_pForumThreadBrowser;

	ForumModel*							m_pModel;
};

#endif//_FORUMWINDOW_H_