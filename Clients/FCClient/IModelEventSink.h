#ifndef _IMODELEVENTSINK_H_
#define _IMODELEVENTSINK_H_

struct IModelEventSink
{
	virtual void OnModelEvent() = 0;
};

#endif//_IMODELEVENTSINK_H_