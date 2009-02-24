#ifndef _FCOBJECTFACTORY_H_
#define _FCOBJECTFACTORY_H_

#include "FCObject.h"

class FCObjectFactory
{
  FCObjectFactory(void);
  ~FCObjectFactory(void);

public:

  static FCObjectFactory& instance();
  static void destroy();

  template <class T>
  T* createObject(FCObject* parent = NULL)
  {
    T* pObj = new T(parent);

    if ( !static_cast<FCObject*>(pObj) )
    {
      delete pObj;
      pObj = NULL;
    }

    return pObj;
  }

private:

  static FCObjectFactory* m_pThis;
};

#endif//_FCOBJECTFACTORY_H_