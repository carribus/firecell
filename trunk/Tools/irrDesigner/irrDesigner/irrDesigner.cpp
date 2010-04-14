// irrDesigner.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "irrDesigner.h"
#include "irrDesignerView.h"

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

#define COL_BACKGROUND  SColor(255, 16,32, 64)

int main(int argc, char** argv)
{
  irrDesignerView view;

  view.create(1024, 768, COL_BACKGROUND);
  return view.exec();
}