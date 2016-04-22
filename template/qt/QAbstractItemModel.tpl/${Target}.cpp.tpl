#include "${Target}.h"

${Target}::${Target}(QObject *parent) :
    QAbstractItemModel(parent)
{
}

QModelIndex ${Target}::index(int row, int column, const QModelIndex &parent) const
{
    // TODO
}

int ${Target}::rowCount(const QModelIndex & parent) const
{
    // TODO
}
 
int ${Target}::columnCount(const QModelIndex & parent) const
{
    // TODO
}

QVariant ${Target}::headerData(int section, Qt::Orientation, int role) const
{
    // TODO
}

// QModelIndex ${Target}::parent(const QModelIndex &child) const
// {
//     // TODO
// }

QVariant ${Target}::data(const QModelIndex &index, int role) const
{
    // TODO
}
