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
#ifndef _FCCONTROLLER_H_
#define _FCCONTROLLER_H_

#include "IModelEventSink.h"
#include "FCModel.h"
#include "FCView.h"
#include "FCViewEvent.h"

class FCController : public IModelEventSink
{

public:

	FCController(void);
	~FCController(void);

	void SetModel(FCModel* pModel);
	void SetView(FCView* pView);

	bool Initialise();
	void Run();

	void OnModelEvent(FCModelEvent event);
	void OnViewEvent(FCViewEvent& event);

private:

	void Process();

	FCModel*					m_pModel;
	FCView*						m_pView;
	bool							m_bRunning;
};

#endif//_FCCONTROLLER_H_