#include <QtCore/QCoreApplication>
#include <jsonparser.h>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    JSONParser parser;
    parser.loadList();

    return 0;
}
