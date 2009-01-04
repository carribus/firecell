#ifndef _FCMODEL_H_
#define _FCMODEL_H_

#include <vector>
#include "IModelEventSink.h"
#include "FCModelEvent.h"

using namespace std;

class FCModel
{
public:

	enum e_ModelState
	{
		None = 0,
		Loading,
		Login,
		Playing,
		ShuttingDown
	};

	FCModel(void);
	~FCModel(void);

	void SubscribeToEvents(IModelEventSink* pSink);
	bool IsSubscribed(IModelEventSink* pSink);

	bool Initialise();

	void SetState(e_ModelState newState);
	e_ModelState GetState()														{ return m_state; }

private:

	void FireEvent(e_FCEventType type, void* pData);

	vector<IModelEventSink*>				m_sinks;
	e_ModelState										m_state;
};

#endif//_FCMODEL_H_