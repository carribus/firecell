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
#include "CharacterSelectionWidget.h"
#include "FCApp.h"

CharacterSelectionWidget::CharacterSelectionWidget(QWidget *parent)
: QWidget(parent)
, m_pChar(NULL)
{
  ui.setupUi(this);

  QPixmap pixmap(FCAPP->getResourceFolder() + "gfx/default_avatar.png");
  ui.lblAvatar->setPixmap(pixmap);

  connect( ui.btnPlay, SIGNAL(clicked()), SLOT(onPlayClicked()) );

}

///////////////////////////////////////////////////////////////////////

CharacterSelectionWidget::~CharacterSelectionWidget()
{

}

///////////////////////////////////////////////////////////////////////

void CharacterSelectionWidget::setCharacter(Character* pChar)
{
  m_pChar = pChar;
  if ( m_pChar )
  {
    ui.stackedWidget->setCurrentIndex(0);
    ui.lblCharName->setText(m_pChar->GetName());
    ui.lblCharLevel->setText(QString("Level %1").arg(m_pChar->GetLevel()));
    ui.lblCharXP->setText(QString("XP: %1").arg(m_pChar->GetXP()));
  }
  else
    ui.stackedWidget->setCurrentIndex(1);
}

///////////////////////////////////////////////////////////////////////

void CharacterSelectionWidget::onPlayClicked()
{
  emit characterSelected( m_pChar->GetID() );
}

///////////////////////////////////////////////////////////////////////
