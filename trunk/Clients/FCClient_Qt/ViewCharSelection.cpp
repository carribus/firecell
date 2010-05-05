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
#include "clientstrings.h"
#include "FCApp.h"
#include "FCPlayerModel.h"
#include "ViewCharSelection.h"

ViewCharSelection::ViewCharSelection(QWidget* parent)
: ViewBase(parent)
{
  ui.setupUi(this);
}

///////////////////////////////////////////////////////////////////////

ViewCharSelection::~ViewCharSelection(void)
{
}

///////////////////////////////////////////////////////////////////////

void ViewCharSelection::setupView()
{
  FCPlayerModel* pPlayerModel = FCAPP->playerModel();
  Character* pChar = NULL;

  int nCount = pPlayerModel->characterCount();

  for ( int i = 0; i < 8; i++ )
  {
    QString strObjName("panelChar%1");
    CharacterSelectionWidget* pCharSel = findChild<CharacterSelectionWidget*>(strObjName.arg(i+1));
    pChar = pPlayerModel->getCharacter(i);

    if ( pCharSel )
    {
      pCharSel->setCharacter(pChar);
      QObject::connect(pCharSel, SIGNAL(characterSelected(FCUINT)), pPlayerModel, SLOT(onCharacterSelected(FCUINT)));
    }
  }
}

