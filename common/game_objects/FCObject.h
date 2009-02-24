#ifndef _FCOBJECT_H_
#define _FCOBJECT_H_

#include <vector>
#include <string>

class FCObject
{
protected:
  FCObject(const std::string& typeName, FCObject* parent = NULL);
  FCObject(const FCObject& src);

public:
  virtual ~FCObject(void);

  virtual FCObject* getParent();
  virtual size_t getChildCount();
  virtual FCObject* getChild(size_t index);
  virtual bool addChild(FCObject* pChild);
  virtual void remove();
  virtual void removeChild(FCObject* pChild);
  virtual void deleteChildren();
  virtual std::string getTypeName();

  void ReadLock();
  void WriteLock();
  void Unlock();

  template <class T> 
  T* getObject()
  {
    return static_cast<T*>(this);
  }

private:

  FCObject*               m_parent;
  std::vector<FCObject*>  m_children;
  std::string             m_typeName;
};

#endif//__FCOBJECT_H_