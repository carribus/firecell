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
 *  \file IServiceLogic.h
    \brief Contains the IServiceLogic interface definition
 */
#ifndef _ISERVICELOGIC_H_
#define _ISERVICELOGIC_H_

/**
 *  \brief The IServiceLogic interface exposes the required functions that a service logic objects must implement
 *  \ingroup Interfaces
 *
 *  The implementation of any specific service logic must implement this interface
 */
struct IServiceLogic
{
  /**
   *  \brief Retrieves the name of the logic object as a string
   */
  virtual const char* GetName() = 0;

  /**
   *  \brief Release the logic object (frees up memory)
   *
   *  Use this function to release the logic object. The object will be deleted, so make sure your pointers
   *  are set to NULL after calling this.
   */
  virtual void Free() = 0;

  /**
   *  \brief Starts the execution of the logic object
   */
  virtual int Start(void) = 0;

  /**
   *  \brief Stops the execution of the logic object
   */
  virtual int Stop(void) = 0;

  /**
   *  \brief Call this function to check if a console is available for output
   *
   *  This function will return <i>true</i> if the service was started as an app, and <i>false</i> if the
   *  service is running as a service/daemon
   */
  virtual void HasConsole(bool bHasConsole) = 0;
};

#endif//_ISERVICELOGIC_H_