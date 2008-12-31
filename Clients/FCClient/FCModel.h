#ifndef _FCMODEL_H_
#define _FCMODEL_H_

#include <vector>
#include "IModelEventSink.h"

using namespace std;

class FCModel
{
public:
	FCModel(void);
	~FCModel(void);

	void SubscribeToEvents(IModelEventSink* pSink);
	bool IsSubscribed(IModelEventSink* pSink);

	bool Initialise();

private:

	vector<IModelEventSink*>				m_sinks;
};

#endif//_FCMODEL_H_