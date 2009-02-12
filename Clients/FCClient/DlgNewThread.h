#ifndef _DLGNEWTHREAD_H_
#define _DLGNEWTHREAD_H_

#include <string>
#include "FCDialog.h"

class DlgNewThread : public FCDialog
{
	DEFINE_IRRLICHT_FORM();

public:
	DlgNewThread(IGUIEnvironment* pEnv, wchar_t* caption, IGUIElement* pParent, s32 id);
	~DlgNewThread(void);

	bool OnButtonClicked(s32 id, IGUIButton* pBtn);

	std::wstring getSubject()												{ return m_strSubject; }
	std::wstring getMessage()												{ return m_strMessage; }

protected:
	void OnOK();
	void OnCancel();

private:

	std::wstring			m_strSubject;
	std::wstring			m_strMessage;
};

#endif//_DLGNEWTHREAD_H_