#ifndef CHARLOCKER_H
#define CHARLOCKER_H

#include <QMap>
#include <QByteArray>

class charLocker
{
    QMap<int,int> char_value_map;
    const char *textPath;
    QByteArray bytes;
public:
    charLocker(QMap<int,int>char_value_map);
    void encrypt();
    void decrypt();
};

#endif // CHARLOCKER_H
