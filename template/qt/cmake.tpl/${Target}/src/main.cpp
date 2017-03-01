#include <QApplication>
#include <QTranslator>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator; 
    // look up e.g. :/translators/myapp_{Locale}.qm
    if (translator.load(QLocale(), QLatin1String("myapp"), QLatin1String("_"), QLatin1String(":/translators"))) {
        a.installTranslator(&translator);
    }

    MainWindow w;
    w.show();

    return a.exec();
}

