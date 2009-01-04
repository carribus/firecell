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
	FCModel				model;
	FCController	controller;
	FCView				view;

	view.SetModel(&model);
	controller.SetModel(&model);
	controller.SetView(&view);

	if ( !model.Initialise() )
		return 1;

	if ( !view.Initialise() )
		return 1;

	if ( !controller.Initialise() )
		return 1;

	controller.Run();

/*
	video::E_DRIVER_TYPE driverType = video::EDT_OPENGL;
	IrrlichtDevice* pDevice = createDevice(driverType, core::dimension2d<s32>(1024, 768), 16, false);

	if ( !pDevice )
		return 1;

	pDevice->setWindowCaption(L"FireCell");
	video::IVideoDriver* pDriver = pDevice->getVideoDriver();
	IGUIEnvironment* pEnv = pDevice->getGUIEnvironment();

	IGUIFont* pFont = pEnv->getFont("./clientdata/fonts/fontcourier.bmp");
	dimension2d<s32> fontDim = pFont->getDimension(L"ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890");

	while ( pDevice->run() && pDriver )
	{
		s32 x1 = 10, y1 = 10;
		if ( pDevice->isWindowActive() )
		{
			pDriver->beginScene( true, true, SColor(255, 128, 0, 0) );

			if ( pFont )
			{
				pFont->draw(L"Initializing subsystems... ", rect<s32>( x1, y1, 100, 40 ), SColor(255, 255, 0, 0) );
				y1 += fontDim.Height;
				pFont->draw(L"Connecting to server... ", rect<s32>( x1, y1, 100, 40), SColor(255, 255, 0, 0) );
				y1 += fontDim.Height;
				pFont->draw(L"Loading assets... ", rect<s32>( x1, y1, 100, 40), SColor(255, 255, 0, 0) );
			}

			pDriver->endScene();
		}
	}

	pDevice->drop();
*/
	return 0;
}