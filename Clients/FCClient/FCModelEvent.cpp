#include "FCModelEvent.h"

FCModelEvent::FCModelEvent(e_FCEventType type, long long data)
: m_eventType(type)
, m_data(data)
{
}

FCModelEvent::~FCModelEvent(void)
{
}
