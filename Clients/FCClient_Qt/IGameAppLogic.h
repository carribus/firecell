#ifndef _IGAMEAPPLOGIC_H_
#define _IGAMEAPPLOGIC_H_

// forward declaration
class QWidget;

/*
 *  The IGameAppLogic interface is used for the objects that will host the view and model(s) for 
 *  the 'in game application windows'. For example, the Forum applications implementation of IGameAppLogic
 *  is as follows:
 *
 *  AppLogicForum implements IGameAppLogic.
 *    AppLogicForum contains both the View class AND the Model class (or reference to) for the Forum application
 *
 *  IGameAppLogic implementation can be thought of as controllers in a sense. They are NOT intended to be a facade.
 *
 */

struct IGameAppLogic
{
  virtual void create(QWidget* parent) = 0;
  virtual void destroy() = 0;
  virtual QWidget* getWidget() = 0;
};

#endif//_IGAMEAPPLOGIC_H_