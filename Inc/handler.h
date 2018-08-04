#ifndef HANDLER_H
#define HANDLER_H

#include <QString>
#include <QTextEdit>

QString baseName(QString str);
char *bytesRead(const char *path, int *byte_number);
void bytesWrite(const char *path, const char *bytes, const int byte_number);

#endif // HANDLER_H
