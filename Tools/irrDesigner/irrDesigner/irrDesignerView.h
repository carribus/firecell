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

  void renderGrid(u32 gapX = 10, u32 gapY = 10);

  bool OnMouseEvent(const SEvent::SMouseInput event);
    bool OnLMouseDown(s32 X, s32 Y);
    bool OnLMouseUp(s32 X, s32 Y);
    bool OnRButtonDown(s32 X, s32 Y);
    bool OnMouseMoved(s32 X, s32 Y);

  bool OnGUIEvent(const SEvent::SGUIEvent event);
    bool OnMenuItemSelected(s32 selectedItem, IGUIContextMenu* pMenu);

private:

  IrrlichtDevice*       m_pDev;
  IVideoDriver*         m_pDriver;
  IGUIEnvironment*      m_pEnv;
  SColor                m_colBackground;
  position2d<s32>       m_lastClickPos;
  bool                  m_bDrawGrid;
  bool                  m_bSnapToGrid;
  bool                  m_bDragging;
  rect<s32>             m_rectRubberBand;
};

#endif//_IRRDESIGNERVIEW_H_