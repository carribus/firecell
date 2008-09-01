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
#ifndef _FCTYPES_H_
#define _FCTYPES_H_

#ifndef NULL
#define NULL 0
#endif//NULL

#define FCBYTE        unsigned char
#define FCWORD        unsigned short
#define FCUSHORT      FCWORD
#define FCDWORD       unsigned long
#define FCUINT        unsigned int

#define FCULONG       FCDWORD
#define FCSHORT       short
#define FCLONG        long
#define FCINT         int

#define FCFLOAT       float
#define FCDOUBLE      double

#define FCCHAR        char
#define FCWCHAR       wchar_t

#define FCSTR         FCCHAR*
#define FCWSTR        FCWCHAR*
#define FCCSTR        const FCSTR
#define FCCWSTR       const FCWSTR

#endif//_FCTYPES_H_