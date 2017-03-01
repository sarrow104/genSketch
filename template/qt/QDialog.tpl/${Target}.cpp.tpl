#include "${Target}.h"

#include <QPaintEvent>

#ifndef VALUE_MSG
#define VALUE_MSG(a) (#a) << '=' << (a)
#endif

${Target}::${Target}(QWidget *parent) :
    QDialog(parent)
{
}

void ${Target}::paintEvent(QPaintEvent* e)
{
    // implement-me
}
