#include <QSpinBox>

#include "${Target}.h"

${Target}::${Target}(QObject *parent) :
    QItemDelegate(parent)
{
}

// NOTE
// 返回new的一个让用户编辑数据用的编辑器(根据需要选择控件，或者自定义)。
// 从model中接受数据，返回用户修改的数据。
QWidget *${Target}::createEditor(QWidget *parent,
                                 const QStyleOptionViewItem & /* option */,
                                 const QModelIndex & /* index */) const
{
    QSpinBox *editor = new QSpinBox(parent);
    editor->setMinimum(0);
    editor->setMaximum(100);
    return editor;
}

// NOTE
// 提供上述组件，在显示时的默认值
void ${Target}::setEditorData(QWidget *editor,
                              const QModelIndex &index) const
{
    // 从选择的model数据中，构造动态生成的编辑器所需要的当前值；
    int value = index.model()->data(index, Qt::EditRole).toInt();
    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
    spinBox->setValue(value);
}

// NOTE
// 将 用户通过动态生成的编辑器，修改后的数据，重新赋值给model的对应index。
void ${Target}::setModelData(QWidget *editor,
                             QAbstractItemModel *model,
                             const QModelIndex &index) const
{
    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
    spinBox->interpretText();
    int value = spinBox->value();
    model->setData(index, value, Qt::EditRole);
}

// NOTE
// 设置编辑器显示尺寸；确保编辑器能完整地显示出来。
void ${Target}::updateEditorGeometry(QWidget *editor,
                                     const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

