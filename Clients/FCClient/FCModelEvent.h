#ifndef _FCMODELEVENT_H_
#define _FCMODELEVENT_H_

enum e_FCEventType
{
	FCME_StateChange,
  FCME_NewCharacterParamsReady,
	FCME_OpenApplication,
	/* Console events */
	FCME_Console_FileSystemInfo,
	FCME_Console_Command,
	/* Forum Events */
  FCME_Forum_CategoriesReceived,
	FCME_Forum_CategoryThreadsReceived,
  FCME_Forum_ThreadContentReceived,
	/* Mission Events */
	FCME_Mission_MissionAccepted,
	FCME_Mission_Completed,
  /* Software Events */
  FCME_Software_SoftwareInstallSuccess,
  FCME_Software_SoftwareInstallFail,
  FCME_Software_SoftwareUninstallSuccess,
  FCME_Software_SoftwareUninstallFail,
	/* XP Event */
	FCME_XP_Gained
};

class FCModelEvent
{
public:
	FCModelEvent(e_FCEventType type, long long data);
	virtual ~FCModelEvent(void);

	e_FCEventType GetType()											{ return m_eventType; }
	long long GetData()													{ return m_data; }

private:

	e_FCEventType					m_eventType;
	long long							m_data;
};

#endif//_FCMODELEVENT_H_