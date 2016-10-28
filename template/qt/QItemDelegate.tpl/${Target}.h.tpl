#ifndef _${Target}_H_${timestamp}_
#define _${Target}_H_${timestamp}_

#include <QItemDelegate>

class ${Target} : public QItemDelegate
{
    Q_OBJECT;
public:
    explicit ${Target}(QObject *parent = 0);
    ~${Target}() = default;

public:
    // createEditor() returns the widget used to change data from the model and
    // can be reimplemented to customize editing behavior.
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    // setEditorData() provides the widget with data to manipulate.
    void setEditorData   (QWidget *editor, const QModelIndex &index) const;

    // updateEditorGeometry() ensures that the editor is displayed correctly
    // with respect to the item view.
    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;
    // setModelData() returns updated data to the model.
    void setModelData    (QWidget *editor, QAbstractItemModel *model,
                          const QModelIndex &index) const;

};

#endif // _${Target}_H_${timestamp}_

