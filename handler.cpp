#include "handler.h"
#include <QStringList>

QString baseName(QString str)
{
    QStringList strlist = str.split("/");
    return strlist.last();
}
