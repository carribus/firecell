#include "FCViewEvent.h"

FCViewEvent::FCViewEvent(e_FCViewEventType type, long long data)
: m_type(type)
, m_data(data)
{
}

FCViewEvent::~FCViewEvent(void)
{
}
