#ifndef _SOCKETSERVER_H_
#define _SOCKETSERVER_H_

#ifdef _WIN32
#include "win/W32SocketServer.h"
#define SocketServer  CW32SocketServer
#else
#include "linux/BSDSocketServer.h"
#define SocketServer  BSDSocketServer
#endif//_WIN32

#endif//_SOCKETSERVER_H_