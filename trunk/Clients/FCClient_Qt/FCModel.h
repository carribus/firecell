#ifndef _FCMODEL_H_
#define _FCMODEL_H_

#include <QObject>

class FCModel : public QObject
{
  Q_OBJECT;

public:

	/*
	 * Model states
	 */
	enum e_ModelState
	{
		NoState = 0,
		LoadingState,
		ConnectingState,
		LoginState,
		CharacterSelectionState,
		PlayingState,
		ShuttingDownState
	};

  FCModel(QObject* parent = 0);
  ~FCModel(void);

	void setState(e_ModelState newState);
  
signals:

  void modelStateChanged(FCModel::e_ModelState newState, FCModel::e_ModelState oldState);

private:

  e_ModelState            m_state;
};

#endif//_FCMODEL_H_