#ifndef _CLIENTSOCKET_H_
#define _CLIENTSOCKET_H_

#ifdef _WIN32
  #include "win/BaseSocket.h"
#else
  #include "linux/BaseSocket.h"
#endif//_WIN32

#endif//_CLIENTSOCKET_H_