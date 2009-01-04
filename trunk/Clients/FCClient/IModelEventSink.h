#ifndef _IMODELEVENTSINK_H_
#define _IMODELEVENTSINK_H_

class FCModelEvent;

struct IModelEventSink
{
	virtual void OnModelEvent(FCModelEvent event) = 0;
};

#endif//_IMODELEVENTSINK_H_