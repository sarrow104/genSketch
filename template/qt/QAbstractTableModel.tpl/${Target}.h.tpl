#ifndef _${Target}_H_${timestamp}_
#define _${Target}_H_${timestamp}_

#include <QAbstractTableModel>

class ${Target} : public QAbstractTableModel
{
    Q_OBJECT;
public:
    explicit ${Target}(QObject *parent = 0);

public:
    // TODO void setXXXX(const XXXX &);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    // TODO
};

#endif // _${Target}_H_${timestamp}_
