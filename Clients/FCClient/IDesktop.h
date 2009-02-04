#ifndef _IDESKTOP_H_
#define _IDESKTOP_H_

class InGameAppWindow;

struct IDesktop
{
  virtual void OnApplicationActivated(InGameAppWindow* pApp) = 0;
};

#endif//_IDESKTOP_H_