#ifndef _FCCONTROLLER_H_
#define _FCCONTROLLER_H_

#include "../../common/fctypes.h"
#include "../../common/PacketExtractor.h"
#include "../../common/PEPacket.h"
#include "../common/Socket/ClientSocket.h"
#include "../common/FCServerObj.h"
#include "IModelEventSink.h"
#include "FCModel.h"
#include "FCView.h"

class FCController : public IModelEventSink
{
public:

	FCController(void);
	~FCController(void);

	void SetModel(FCModel* pModel);
	void SetView(FCView* pView);

	bool Initialise();
	void Run();

	void OnModelEvent(FCModelEvent event);

private:

	void Process();

	FCModel*					m_pModel;
	FCView*						m_pView;
	bool							m_bRunning;

	BaseSocket				m_sock;
	FCServerObj				m_server;
};

#endif//_FCCONTROLLER_H_