#ifndef _VIEWLOGICLOADING_H_
#define _VIEWLOGICLOADING_H_

#include <string>
#include <vector>
#include "IViewLogic.h"

using namespace std;
using namespace irr::scene;
using namespace irr::gui;

class ViewLogicLoading : public IViewLogic
{
public:
	ViewLogicLoading();
	~ViewLogicLoading(void);

	/*
	 *	IViewLogic implementation
	 */
	void Create(FCView* pContainer, IrrlichtDevice* pDevice);
	void Destroy();
	void SetActive();
	void Refresh();
  void OnModelStateChange(FCModel::StateInfo state);
	SColor GetBackgroundColour()													{ return SColor(255, 64, 64, 64); }

private:

	FCView*											m_pContainer;
	IrrlichtDevice*							m_pDevice;
	ISceneManager*							m_pScene;
	IGUIEnvironment*						m_pEnv;

	IGUIStaticText*							m_pTextObject;

	std::basic_string<wchar_t>							    m_strDetails;
	vector< std::basic_string< wchar_t > >			m_arrDetailLines;
};

#endif//_VIEWLOGICLOADING_H_