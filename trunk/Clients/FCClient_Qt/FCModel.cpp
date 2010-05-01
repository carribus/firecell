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