#include "${Target}.h"

${Target}::${Target}(QObject *parent) :
    QAbstractTableModel(parent)
{
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

// QString ${Target}::currencyAt(int offset) const
// {
//     // TODO
// }
// 
// void ${Target}::setCurrencyMap(const QMap<QString, double> &map)
// {
//     // TODO
// }

QVariant ${Target}::data(const QModelIndex &index, int role) const
{
    // TODO
}
