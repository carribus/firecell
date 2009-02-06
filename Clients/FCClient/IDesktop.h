#ifndef _IDESKTOP_H_
#define _IDESKTOP_H_

class DesktopIcon;
class InGameAppWindow;

struct IDesktop
{
	virtual void OnDesktopIconSelected(DesktopIcon* pIcon) = 0;
  virtual void OnApplicationActivated(InGameAppWindow* pApp) = 0;
};

#endif//_IDESKTOP_H_