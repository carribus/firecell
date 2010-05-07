/*
    FireCell Qt Client - The Qt client code for the firecell multiplayer game
    Copyright (C) 2008-2010  Peter M. Mares

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
#ifndef _VIEWGAME_H_
#define _VIEWGAME_H_

#include "ViewBase.h"

class ViewGame : public ViewBase
{
public:
  ViewGame(QWidget* parent);
  ~ViewGame(void);

  /*
   * ViewBase implementation
   */
  void setupView();

protected:

  void paintEvent(QPaintEvent* event);

private:

  QPixmap         m_background;
};

#endif//_VIEWGAME_H_