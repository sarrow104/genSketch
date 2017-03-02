#include "${Target}.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ${Target} w;
    w.show();

    return a.exec();
}
