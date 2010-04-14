#ifndef _ELEMENTCONTAINER_H_
#define _ELEMENTCONTAINER_H_

class ElementContainer : public IGUIElement
{
public:
  ElementContainer(IGUIEnvironment* pEnv, IGUIElement* pParent, s32 id = -1, rect<s32> r = rect<s32>(0, 0, 10, 10));
  ~ElementContainer(void);

  void draw();
};

#endif//_ELEMENTCONTAINER_H_