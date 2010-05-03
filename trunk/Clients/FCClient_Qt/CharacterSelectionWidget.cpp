#include "CharacterSelectionWidget.h"

CharacterSelectionWidget::CharacterSelectionWidget(QWidget *parent)
: QWidget(parent)
, m_pChar(NULL)
{
  ui.setupUi(this);

  QPixmap pixmap("./clientdata/gfx/default_avatar.png");
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
