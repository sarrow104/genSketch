#ifndef _${Target}_H_${timestamp}_
#define _${Target}_H_${timestamp}_

#include <QAbstractItemModel>

class ${Target} : public QAbstractItemModel
{
    Q_OBJECT;
public:
    explicit ${Target}(QObject *parent = 0);

public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    // QModelIndex parent(const QModelIndex &child) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    // TODO
};

#endif // _${Target}_H_${timestamp}_
