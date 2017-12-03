#ifndef CHARMAP_H
#define CHARMAP_H

#include <QMap>
class charMap
{
    QMap<int,int> char_value_map;
    int limit;

public:
    const char *filepath;

    charMap(char *str);
    int myRandom(int limit);
    //create map and write to local file
    void create_map();
    //read map from local file.
    void read_map();
    void value_move(int value);
    QMap<int,int> getMap();
};

#endif // CHARMAP_H
