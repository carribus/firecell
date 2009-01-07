#ifndef _FCMODEL_H_
#define _FCMODEL_H_

#include <vector>
#include "../../common/fctypes.h"
#include "../../common/PacketExtractor.h"
#include "../../common/PEPacket.h"
#include "../common/Socket/ClientSocket.h"
#include "../common/FCServerObj.h"
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
		Connecting,
		Login,
		Playing,
		ShuttingDown
	};

	struct StateInfo
	{
		e_ModelState state;
		FCSHORT stateStep;
	};

	FCModel(void);
	~FCModel(void);

	void SubscribeToEvents(IModelEventSink* pSink);
	bool IsSubscribed(IModelEventSink* pSink);

	bool Initialise();
	void SetState(e_ModelState newState);
	StateInfo GetState()														{ return m_state; }

	bool ProcessData();

private:

	bool LoadResources();

	void FireEvent(e_FCEventType type, void* pData);

	vector<IModelEventSink*>				m_sinks;
	StateInfo												m_state;

	BaseSocket				m_sock;
	FCServerObj				m_server;
};

#endif//_FCMODEL_H_