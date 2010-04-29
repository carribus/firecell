#include "FCModel.h"

FCModel::FCModel(QObject* parent)
: QObject(parent)
, m_state(NoState)
{
}

///////////////////////////////////////////////////////////////////////

FCModel::~FCModel(void)
{
}

///////////////////////////////////////////////////////////////////////

void FCModel::setState(e_ModelState newState)
{
  e_ModelState oldState = m_state;
  m_state = newState;

  emit modelStateChanged(m_state, oldState);
}
