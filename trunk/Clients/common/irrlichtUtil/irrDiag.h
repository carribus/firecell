#ifndef _IRRDIAG_H_
#define _IRRDIAG_H_

#include <irrlicht.h>

using namespace irr;
using namespace video;
using namespace gui;

class irrDiag
{
public:

  irrDiag(void);
  ~irrDiag(void);

  std::string dumpElementTree(IGUIElement* pStartNode);

private:

  std::string irrDiag::enumerateElements(IGUIElement* pParentNode, u32 level = 0);
};

#endif//_IRRDIAG_H_