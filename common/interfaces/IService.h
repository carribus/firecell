/*
    FireCell Server - The server code for the firecell multiplayer game
    Copyright (C) 2008  Peter M. Mares

		Contact: carribus@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/**
 *  \file IService.h
    \brief Contains the IService interface definition
    \defgroup Interfaces Abstract Interfaces
 */
#ifndef _ISERVICE_H_
#define _ISERVICE_H_

struct IServiceLogic;

/**
 *  \brief The IService interface exposes a Win32 Service or *nix Daemon core functionality
 *  \ingroup Interfaces
 *
 *  The implementation of both CW32Service and CDaemon implement this interface, thereby abstracting
 *  the basic semantics of creating and using a win32 server/*nix daemon standard.
 */
struct IService
{
  /**
   *  \brief Attaches the functional logic that this service/daemon will be hosting
   *
   *  Use this function to attach the class that implements the logic of the service/daemon to the implementation.
   *  NOTE: The implementation object of the IServiceLogic interface should attempt to remain as platform neutral as possible.
   */
  virtual bool ISRV_AttachLogic(IServiceLogic* pLogic) = 0;

  /**
   *  \brief Make the service run as an app (in console)
   *
   *  Use this function to flag the logic to run as a standard application.
   */
  virtual void ISRV_RunAsApp(bool bAsApp) = 0;

  /**
   *  \brief Starts the service
   */
  virtual int ISRV_Run(void* pData) = 0;

  /**
   *  \brief Stops the service
   */
  virtual int ISRV_Stop() = 0;

  /**
   *  \brief Retrieves the path of the executing binary
   */
  virtual void ISRV_GetBinPath(char* outBuffer, size_t& bufferSize) = 0;
};

#endif//_ISERVICE_H_