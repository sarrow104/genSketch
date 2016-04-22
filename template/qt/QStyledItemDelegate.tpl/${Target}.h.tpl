#ifndef _${Target}_H_${timestamp}_
#define _${Target}_H_${timestamp}_

#include <QStyledItemDelegate>

class ${Target} : public QStyledItemDelegate
{
    Q_OBJECT;
public:
    explicit ${Target}(QObject *parent = 0);
    ~${Target}() = default;

public:
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData   (QWidget *editor, const QModelIndex &index) const;

    void setModelData    (QWidget *editor, QAbstractItemModel *model,
                          const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;
};

#endif // _${Target}_H_${timestamp}_
