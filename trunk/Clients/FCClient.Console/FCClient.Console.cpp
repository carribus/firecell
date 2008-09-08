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

#include <stdio.h>
#include <string>
#include "FCController.h"
#include "FCDisplay.h"

using namespace std;

#define VER_MAJOR     0
#define VER_MINOR     1

void ShowInfo()
{
  printf("FireCell (Console client v%ld.%02ld)\n", VER_MAJOR, VER_MINOR);
  printf("Copyright 2008 (c) Peter Mares\n");
  printf("-------------------------------\n\n");
}

///////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
  ShowInfo();

  FCController game;
  FCDisplay view;

  game.Initialise();
  game.Run();


	return 0;
}

