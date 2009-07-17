#ifndef _IAPPBARAPPLET_H_
#define _IAPPBARAPPLET_H_

#include <string>

struct IAppBarApplet
{
  virtual std::wstring getHoverText() = 0;
  virtual void drawAppletIcon(core::rect<s32>& rect) = 0;
  virtual void onAppletClicked() = 0;
  virtual void OnAppletRightClicked() = 0;
};

#endif//_IAPPBARAPPLET_H_