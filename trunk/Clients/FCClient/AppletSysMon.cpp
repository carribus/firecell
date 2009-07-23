#include <sstream>
#include "../common/fccutils.h"
#include "FCModel.h"
#include "AppletSysMon.h"

#define ICON_FILE "./clientdata/gfx/applets/sysmon.jpg"

AppletSysMon::AppletSysMon(DesktopAppBar* pParent, IGUIEnvironment* pEnv)
: AppBarAppletBase(pParent, pEnv, ICON_FILE)
{
	#ifdef _DEBUG
	setDebugName("AppletSysMon");
	#endif
}

//////////////////////////////////////////////////////////////////////////////////////////

AppletSysMon::~AppletSysMon(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////

std::wstring AppletSysMon::getHoverText()
{
  std::wstringstream ss;
  FCModel& model = FCModel::instance();
  ComputerBase& comp = model.GetPlayer()->GetComputer();
  FCSHORT cpuLeft = comp.GetAvailableCPU();
  FCSHORT cpuMax = comp.GetProcessor().GetCoreCount() * comp.GetProcessor().GetCoreSpeed();
  FCULONG memLeft = comp.GetAvailableMemory();
  FCULONG memMax = comp.GetMemory().GetMemorySize();

  ss.precision(3);
  ss << L"System Monitor\nCPU: " << 
        (double)(((double)(cpuMax-cpuLeft) / (double)cpuMax) * 100L) << 
        L"%\nMemory: " << 
        formatCurrencyValue( "", memMax-memLeft ).c_str() << L"MB / " << 
        formatCurrencyValue( "", memMax ).c_str() << L"MB";
  return ss.str();
}

//////////////////////////////////////////////////////////////////////////////////////////

void AppletSysMon::onAppletClicked()
{
}

//////////////////////////////////////////////////////////////////////////////////////////

void AppletSysMon::OnAppletRightClicked()
{
}

//////////////////////////////////////////////////////////////////////////////////////////

void AppletSysMon::onAppletHover(bool bHover)
{
  setToolTipText( getHoverText().c_str() );
}

//////////////////////////////////////////////////////////////////////////////////////////

void AppletSysMon::draw()
{
  IVideoDriver* pVideo = m_pEnv->getVideoDriver();
  FCModel& model = FCModel::instance();
  ComputerBase& comp = model.GetPlayer()->GetComputer();
  FCSHORT cpuLeft = comp.GetAvailableCPU();
  FCSHORT cpuMax = comp.GetProcessor().GetCoreCount() * comp.GetProcessor().GetCoreSpeed();
  FCULONG memLeft = comp.GetAvailableMemory();
  FCULONG memMax = comp.GetMemory().GetMemorySize();
  core::rect<s32> memRect, cpuRect;

  AppBarAppletBase::draw();

  memRect = cpuRect = AbsoluteRect;
  memRect.LowerRightCorner.X = memRect.UpperLeftCorner.X + (memRect.getWidth()/2);
  cpuRect.UpperLeftCorner.X = memRect.LowerRightCorner.X;

  memRect.UpperLeftCorner.Y = memRect.LowerRightCorner.Y - ( (double)memRect.getHeight() * ((double)((memMax-memLeft) / (double)memMax) ) );
  cpuRect.UpperLeftCorner.Y = cpuRect.LowerRightCorner.Y - ( (double)cpuRect.getHeight() * ((double)((cpuMax-cpuLeft) / (double)cpuMax) ) );

  pVideo->draw2DRectangle( SColor(128, 255, 0, 0), memRect, &AbsoluteClippingRect );
  pVideo->draw2DRectangle( SColor(128, 0, 0, 255), cpuRect, &AbsoluteClippingRect );
}