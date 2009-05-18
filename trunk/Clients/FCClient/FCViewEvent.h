#ifndef _FCVIEWEVENT_H_
#define _FCVIEWEVENT_H_

#include "../../common/fctypes.h"
#include <string>

using namespace std;

enum e_FCViewEventType
{
	VE_None,
	VE_ClientExit,
	VE_Login,
	VE_CharacterSelected,
  VE_NewCharacterRequested,
	VE_DesktopOptionActivated,
	/* Console Events */
	VE_ConRefresh,
	VE_ConCommandIssued,
  /* Forum Events */
  VE_ForumRefreshCategories,
	VE_ForumCategorySelected,
  VE_ForumNewThread,
  VE_ForumThreadSelected,
	/* Mission Events */
	VE_MissionAccepted,
  /* Software Events */
  VE_InstallSoftware,
  VE_UninstallSoftware,
  VE_EnablePort
};

/////////////////////////////////////////////////////////////////

class FCViewEvent
{
public:
	FCViewEvent(e_FCViewEventType type, long long data = 0);
	~FCViewEvent(void);

	e_FCViewEventType				GetType()													{ return m_type; }
	long long								GetData()													{ return m_data; }

private:

	e_FCViewEventType				m_type;
	long long								m_data;
};

/////////////////////////////////////////////////////////////////

class FCViewEventLogin : public FCViewEvent
{
public:
	FCViewEventLogin(const wstring& username, const wstring& password)
		: FCViewEvent( VE_Login )
		, m_username(username)
		, m_password(password)
	{
	}

	wstring GetUsername()											{ return m_username; }
	wstring GetPassword()											{ return m_password; }

private:

	wstring									m_username;
	wstring									m_password;
};

/////////////////////////////////////////////////////////////////

class FCViewEventNewForumPost : public FCViewEvent
{
public:
  FCViewEventNewForumPost(FCULONG category_id, FCULONG thread_id, const wstring& subject, const wstring& message)
    : FCViewEvent( VE_ForumNewThread )
    , m_category_id(category_id)
		, m_thread_id(thread_id)
    , m_subject(subject)
    , m_message(message)
  {
  }

  FCULONG getCategoryID()                    { return m_category_id; }
	FCULONG getThreadID()											 { return m_thread_id; }
  wstring getSubject()                       { return m_subject; }
  wstring getMessage()                       { return m_message; }

private:

  wstring                 m_subject;
  wstring                 m_message;
  FCULONG                 m_category_id;
	FCULONG									m_thread_id;
};

/////////////////////////////////////////////////////////////////

class FCViewEventThreadSelected : public FCViewEvent
{
public:
  FCViewEventThreadSelected(FCULONG category_id, FCULONG thread_id)
    : FCViewEvent( VE_ForumThreadSelected )
    , m_category_id(category_id)
    , m_thread_id(thread_id)
  {
  }

  FCULONG getCategoryID()                   { return m_category_id; }
  FCULONG getThreadID()                     { return m_thread_id; }

private:

  FCULONG                 m_category_id;
  FCULONG                 m_thread_id;
};

/////////////////////////////////////////////////////////////////

class FCViewEventInstallSoftware : public FCViewEvent
{
public:
  FCViewEventInstallSoftware(FCULONG itemID, FCSHORT portNum)
    : FCViewEvent(VE_InstallSoftware)
    , m_itemID(itemID)
    , m_portNum(portNum)
  {
  }

  FCULONG getItemID()                       { return m_itemID; }
  FCSHORT getPortNum()                      { return m_portNum; }

private:

  FCULONG m_itemID;
  FCSHORT m_portNum;
};

/////////////////////////////////////////////////////////////////

class FCViewEventEnableNetworkPort : public FCViewEvent
{
public:
  FCViewEventEnableNetworkPort(FCSHORT portNum, bool bEnabled)
    : FCViewEvent(VE_EnablePort)
    , m_portNum(portNum)
    , m_bEnabled(bEnabled)
  {
  }

  FCSHORT getPortNum()                        { return m_portNum; }
  bool isEnabled()                            { return m_bEnabled; }

private:

  FCSHORT m_portNum;
  bool m_bEnabled;
};

#endif//_FCVIEWEVENT_H_