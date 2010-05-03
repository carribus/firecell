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

