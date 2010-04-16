#ifndef _ELEMENTCONTAINER_H_
#define _ELEMENTCONTAINER_H_

class ElementContainer : public IGUIElement
{
  enum ContainerState
  {
    ECS_None,
    ECS_Selected,
    ECS_Hovered
  };

public:
  ElementContainer(IGUIEnvironment* pEnv, IGUIElement* pParent, s32 id = -1, rect<s32> r = rect<s32>(0, 0, 10, 10));
  ~ElementContainer(void);

  void draw();
  bool OnEvent(const SEvent& event);

  void setObject(IGUIElement* pElement, const wchar_t* elemName);

  ContainerState getState()                       { return m_state; }

private:

  ContainerState        m_state;
};

#endif//_ELEMENTCONTAINER_H_