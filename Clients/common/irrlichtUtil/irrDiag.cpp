#include <sstream>
#include "irrDiag.h"

irrDiag::irrDiag(void)
{
}

////////////////////////////////////////////////////////////////////////////////////

irrDiag::~irrDiag(void)
{
}

////////////////////////////////////////////////////////////////////////////////////

std::string irrDiag::dumpElementTree(IGUIElement* pStartNode)
{
  if ( !pStartNode )
    return "";

  std::string result;

  result = enumerateElements(pStartNode);

  return result;
}

////////////////////////////////////////////////////////////////////////////////////

std::string irrDiag::enumerateElements(IGUIElement* pParentNode, u32 level)
{
  if ( !pParentNode )
    return "";

  std::stringstream ss;
  IGUIElement* pChild = NULL;
  core::list<IGUIElement*> children = pParentNode->getChildren();
  core::list<IGUIElement*>::Iterator it = children.begin();
  core::list<IGUIElement*>::Iterator limit = children.end();
  const c8* pElemName = pParentNode->getDebugName();

  if ( !pElemName )
    pElemName = "<Unnamed Element>";

  for ( u32 i = 0; i < level; i++ )
    ss << "\t";
  ss << "[" << pParentNode->getID() << "] " << pElemName << "\n";

  for ( ; it != limit; it++ )
  {
    pChild = (*it);
    ss << enumerateElements(pChild, level+1);
  }

  return ss.str();
}