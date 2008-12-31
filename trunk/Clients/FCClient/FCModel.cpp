#include "FCModel.h"

FCModel::FCModel(void)
{
}

///////////////////////////////////////////////////////////////////////

FCModel::~FCModel(void)
{
}

///////////////////////////////////////////////////////////////////////

void FCModel::SubscribeToEvents(IModelEventSink* pSink)
{
	if ( !IsSubscribed(pSink) )
		m_sinks.push_back(pSink);
}

///////////////////////////////////////////////////////////////////////

bool FCModel::IsSubscribed(IModelEventSink* pSink)
{
	vector<IModelEventSink*>::iterator it = m_sinks.begin();

	while ( it != m_sinks.end() )
	{
		if ( (*it) == pSink )
			return true;
		it++;
	}

	return false;
}

///////////////////////////////////////////////////////////////////////

bool FCModel::Initialise()
{
	return false;
}