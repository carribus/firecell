#ifndef _FORUMWINDOW_H_
#define _FORUMWINDOW_H_

#include <string>
#include "FCModel.h"
#include "ForumModel.h"
#include "InGameAppWindow.h"
#include "GUIForumCatBrowser.h"

class ForumWindow : public InGameAppWindow
									, public IForumCatBrowserSink
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

	/*
	 *	IForumCatBrowserSink implementation
	 */
	void OnCategorySelected(FCULONG category_id);

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

	ForumModel*							m_pModel;
};

#endif//_FORUMWINDOW_H_