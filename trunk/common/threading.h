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
#ifndef _THREADING_H_
#define _THREADING_H_

//
// include the appropriate library for threading based on OS
//
#ifdef _WIN32
  #include "pthreads-win32/include/pthread.h"
  #include "pthreads-win32/include/sched.h"
  // include the the Visual Studio compatible Structured Exception handling version of the pthreads-win32 library
  #pragma comment(lib, "../common/pthreads-win32/lib/pthreadVSE2.lib")
#else
  #include <pthread.h>
  #include <sched.h>
#endif
#endif//_THREADING_H_