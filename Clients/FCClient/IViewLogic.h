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
#ifndef _IVIEWLOGIC_H_
#define _IVIEWLOGIC_H_

#include <irrlicht.h>
#include "FCModel.h"

class FCView;

using namespace irr;
using namespace video;

struct IViewLogic
{
	virtual void Create(FCView* pContainer, IrrlichtDevice* pDevice) = 0;
	virtual void Destroy() = 0;
	virtual void SetActive() = 0;
	virtual void Refresh() = 0;
  virtual bool OnModelEvent(FCModelEvent event) = 0;
	virtual SColor GetBackgroundColour() = 0;
  virtual FCView* GetContainer() = 0;
};

#endif//_IVIEWLOGIC_H_