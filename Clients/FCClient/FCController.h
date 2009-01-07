#ifndef _FCCONTROLLER_H_
#define _FCCONTROLLER_H_

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
};

#endif//_FCCONTROLLER_H_