#ifndef _NETWORKPORTS_H_
#define _NETWORKPORTS_H_

#include <map>
#include "../fctypes.h"

class NetworkPorts
{
public:
	NetworkPorts(void);
	~NetworkPorts(void);

private:

	std::map<FCSHORT, bool>				m_ports;
};

#endif//_NETWORKPORTS_H_