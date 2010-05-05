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
#include "StdAfx.h"
#include "FCModel.h"

FCModel::FCModel(QObject* parent)
: QObject(parent)
{
}

///////////////////////////////////////////////////////////////////////

FCModel::~FCModel(void)
{
}

///////////////////////////////////////////////////////////////////////
/*
void FCModel::setState(e_ModelState newState)
{

  e_ModelState oldState = m_state.state;
  m_state.state = newState;
	m_state.stateStep = 0;

  emit modelStateChanged(m_state.state, oldState);
}

///////////////////////////////////////////////////////////////////////

void FCModel::SetStateStep(FCSHORT stateStep)
{
  e_ModelState oldState = m_state.state;
  m_state.stateStep = stateStep;
  emit modelStateChanged(m_state.state, oldState);
}
*/
