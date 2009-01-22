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

#include <irrlicht.h>
#include "FCModel.h"
#include "FCController.h"
#include "FCView.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

int main(int argc, char** argv)
{
  FCModel&			model = FCModel::instance();
	FCController	controller;
	FCView				view;

	view.SetModel(&model);
	controller.SetModel(&model);
	controller.SetView(&view);

	if ( !view.Initialise() )
		return 1;

  if ( !model.Initialise() )
		return 1;

	if ( !controller.Initialise() )
		return 1;

	controller.Run();

	return 0;
}