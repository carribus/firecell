#ifndef _SOFTWARE_SERVICE_H_
#define _SOFTWARE_SERVICE_H_

#include "../common/fctypes.h"

class SoftwareService
{
public:
	SoftwareService(FCSHORT port);
	virtual ~SoftwareService(void);

	FCSHORT getPort()													{ return m_port; }
	FCULONG getHealth()												{ return m_health; }

private:

	FCSHORT					m_port;
	FCULONG					m_health;
};

#endif//_SOFTWARE_SERVICE_H_