#ifndef FORUMVIEW_H
#define FORUMVIEW_H

#include <QWidget>
#include "ui_ForumView.h"

class ForumView : public QWidget
{
    Q_OBJECT

public:
    ForumView(QWidget *parent = 0);
    ~ForumView();

    void setModel(QAbstractItemModel* pModel);

private:
    Ui::ForumViewClass ui;
};

#endif // FORUMVIEW_H
