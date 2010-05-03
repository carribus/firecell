#ifndef CHARACTERSELECTIONWIDGET_H
#define CHARACTERSELECTIONWIDGET_H

#include <QWidget>
#include "ui_CharacterSelectionWidget.h"
#include "./dataobjects/Character.h"

class CharacterSelectionWidget : public QWidget
{
  Q_OBJECT

public:
  CharacterSelectionWidget(QWidget *parent = 0);
  ~CharacterSelectionWidget();

  void setCharacter(Character* pChar);
  Character* getCharacter()                             { return m_pChar; }

signals:
  void characterSelected(FCUINT charID);

protected slots:
  void onPlayClicked();

private:
  Ui::CharacterSelectionWidgetClass ui;
  Character* m_pChar;
};

#endif // CHARACTERSELECTIONWIDGET_H
