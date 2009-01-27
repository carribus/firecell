/**
 *  \ingroup DynLog
 */
#ifndef _REFCOUNTER_H_
#define _REFCOUNTER_H_

namespace Logging
{

class IRefCounter
{
public:

  //! Constructor
  //! Sets the reference count to 1 at object instantiation
  IRefCounter()
    : m_refCount(1)
  {
  }

  //! Destructor
  virtual ~IRefCounter()
  {
  }

  /**
   *  \brief Add a reference to the object. This should be done whenever a new resource acquires this object.
   *  
   *  Each call to addRef() should be matched with a call to release() once the reference owner is done using
   *  the object. Failure to do so will result in memory leaks.
   */
  void addRef()
  {
    m_refCount++;
  }

  /**
   *  \brief Free a reference to the object. This should be done whenever a consumer of the object is done using it.
   */
  bool release()
  {
    if ( m_refCount > 0 )
    {
      --m_refCount;
      if ( !m_refCount )
      {
        delete this;
        return true;
      }
    }
    
    return false;
  }

private:

  mutable size_t      m_refCount;
};

}; // end of Logging namesapce

#endif//_REFCOUNTER_H_