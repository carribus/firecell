#ifndef _IRRDESIGNERVIEW_H_
#define _IRRDESIGNERVIEW_H_

class irrDesignerView : public IEventReceiver
{
public:
  irrDesignerView(void);
  ~irrDesignerView(void);

  bool create(s32 width, s32 height, SColor background = SColor(255, 0, 0, 0));
  u32 exec();

  virtual bool OnEvent(const SEvent& event);

protected:

  bool OnMouseEvent(const SEvent::SMouseInput event);
    bool OnRButtonDown(s32 X, s32 Y);

  bool OnGUIEvent(const SEvent::SGUIEvent event);

private:

  IrrlichtDevice*       m_pDev;
  IVideoDriver*         m_pDriver;
  IGUIEnvironment*      m_pEnv;
  SColor                m_colBackground;
};

#endif//_IRRDESIGNERVIEW_H_