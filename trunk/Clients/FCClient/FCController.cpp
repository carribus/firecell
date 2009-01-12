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
#include "../common/ResourceManager.h"
#include "FCController.h"

FCController::FCController(void)
: m_pModel(NULL)
, m_pView(NULL)
, m_bRunning(false)
{
}

///////////////////////////////////////////////////////////////////////

FCController::~FCController(void)
{
}

///////////////////////////////////////////////////////////////////////

void FCController::SetModel(FCModel* pModel)
{
	m_pModel = pModel;
	pModel->SubscribeToEvents(this);
}

///////////////////////////////////////////////////////////////////////

void FCController::SetView(FCView* pView)
{
	m_pView = pView;
	pView->SetController(this);
}

///////////////////////////////////////////////////////////////////////

bool FCController::Initialise()
{
	return true;
}

///////////////////////////////////////////////////////////////////////

void FCController::Run()
{
	if ( !m_pModel || !m_pView )
		return;

	m_bRunning = true;
	while ( m_bRunning )
	{
		if ( m_pModel->ProcessData() )
    {
		  m_bRunning = m_pView->Update();
    }
    else
      break;
	}
}

///////////////////////////////////////////////////////////////////////

void FCController::OnModelEvent(FCModelEvent event)
{

}

///////////////////////////////////////////////////////////////////////

void FCController::OnViewEvent(FCViewEvent& event)
{
	switch ( event.GetType() )
	{
	case	VE_None:
		break;

	case	VE_ClientExit:
		m_pModel->SetState( FCModel::ShuttingDown );
		break;

	case	VE_Login:
		FCViewEventLogin* pEv = (FCViewEventLogin*) &event;
		m_pModel->StartLogin( pEv->GetUsername(), pEv->GetPassword() );
		break;
	}
}