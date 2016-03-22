#include "${Target}.h"

${Target}::${Target}(QPushButton *parent) : QPushButton(parent)
{
    connect(this, &${Target}::clicked,
            this, &${Target}::onButtonCliecked);
}

void ${Target}::onButtonCliecked()
{
    // TODO
}
