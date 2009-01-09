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

class FCModel : public IBaseSocketSink
{
public:

	/*
	 * Model states
	 */
	enum e_ModelState
	{
		None = 0,
		Loading,
		Connecting,
		Login,
		Playing,
		ShuttingDown
	};

	/*
	 *	Sub-states for each model state
	 */

	// e_ModelState::Loading
	enum e_ModelStateLoading
	{
    MS_Loading_None,
    MS_Loading_Text,
    MS_Loading_Graphics,
    MS_Loading_Sounds
	};

  // e_ModelState::Connecting
  enum e_ModelStateConnecting
  {
    MS_Connecting_None,
    MS_Connecting_Connecting,
    MS_Connecting_Connected,
    MS_Connecting_Retry,
    MS_Connecting_FinalFail
  };

  /*
   *  StateInfo structure that is passed along with the FCME_StateChange event
   */
	struct StateInfo
	{
		e_ModelState state;
		FCSHORT stateStep;
	};

  /* */
	FCModel(void);
	~FCModel(void);

	void SubscribeToEvents(IModelEventSink* pSink);
	bool IsSubscribed(IModelEventSink* pSink);

	bool Initialise();
	void SetState(e_ModelState newState);
  void SetStateStep(FCSHORT stateStep);
	StateInfo GetState()														{ return m_state; }

	bool ProcessData();

	/*
	 * IBaseSocketSink implementation
	 */
	void OnAccepted(BaseSocket* pSocket, int nErrorCode);
	void OnConnected(BaseSocket* pSocket, int nErrorCode);
	void OnDisconnected(BaseSocket* pSocket, int nErrorCode);
	void OnDataReceived(BaseSocket* pSocket, FCBYTE* pData, int nLen);

private:

	bool LoadResources();

	bool ConnectToServer();

	void FireEvent(e_FCEventType type, void* pData);

	vector<IModelEventSink*>				m_sinks;
	StateInfo												m_state;

	BaseSocket				m_sock;
	FCServerObj				m_server;
	bool							m_bConnected;
  FCSHORT           m_connectRetry;
};

#endif//_FCMODEL_H_