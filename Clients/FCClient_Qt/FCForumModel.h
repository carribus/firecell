#ifndef FCFORUMMODEL_H
#define FCFORUMMODEL_H

#include <QAbstractItemModel>

class FCForumModel : public QAbstractItemModel
{
  Q_OBJECT

public:
    FCForumModel(QObject *parent);
    ~FCForumModel();

private:
    
};

#endif // FCFORUMMODEL_H
