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

	void OnModelEvent() {};

private:

	FCModel*					m_pModel;
	FCView*						m_pView;
};

#endif//_FCCONTROLLER_H_