#ifndef _FCVIEWEVENT_H_
#define _FCVIEWEVENT_H_

#include <string>

using namespace std;

enum e_FCViewEventType
{
	VE_None,
	VE_ClientExit,
	VE_Login
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

#endif//_FCVIEWEVENT_H_