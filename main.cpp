#include "pkgbuildassistant.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PKGBUILDASSISTANT w;
\\ test
    w.show();
    return a.exec();
}
