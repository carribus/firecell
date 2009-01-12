#ifndef _FCMODELEVENT_H_
#define _FCMODELEVENT_H_

enum e_FCEventType
{
	FCME_StateChange
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