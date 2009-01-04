#ifndef _QUEUEREADERTHREAD_H_
#define _QUEUEREADERTHREAD_H_

#include <deque>
#include "PThreadWrapper.h"

template <class CallBackObjectType, class QueueObjectType>
class QueueReaderThread :	public PThreadWrapper< QueueReaderThread< CallBackObjectType, QueueObjectType > >
{
	DECLARE_THREAD_CLASS(QueueReaderThread);

public:

	struct ThreadContext
	{
		typedef void (CallBackObjectType::*QER_CALLBACK_FUNC)(QueueObjectType& qo);

		deque< QueueObjectType >* pSourceQueue;
		CallBackObjectType* pCallbackObject;
		QER_CALLBACK_FUNC pCallback;
	};

	QueueReaderThread(void)
	{
	}

	~QueueReaderThread(void)
	{
	}

protected:

	static void* RunThread(void* pData)
	{
		QueueReaderThread* pThis = (QueueReaderThread*) pData;
		ThreadContext* pCtx = (ThreadContext*) pThis->GetData();

		if ( !pThis || !pCtx )
			return NULL;

		while ( pThis->IsRunning() )
		{
			while ( pCtx->pSourceQueue->size() )
			{
				QueueObjectType qo = pCtx->pSourceQueue->front();
				pCtx->pSourceQueue->pop_front();

				(pCtx->pCallbackObject->*pCtx->pCallback)(qo);
			}
#ifdef _WIN32
	    Sleep(250);
#else
		  usleep(250000);
#endif		
		}

		return 0;
	}
};

#endif//_QUEUEREADERTHREAD_H_