#include <stdlib.h>
#include <time.h>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "charmap.h"
#include "log.h"

CharMap::CharMap(const char *str) : filepath(str)
{
    srand(time(NULL));
    limit = (1<<8)-1;
}

int CharMap::myRandom(int limit)
{
#ifdef Q_OS_WIN
    return int((double)rand()/RAND_MAX*limit+0.5);
#else
    return int((double)random()/RAND_MAX*limit+0.5);
#endif
}

void CharMap::create_map()
{
    if(0 != access(filepath,F_OK)){  // there is no encrypt code file.
        while(char_value_map.size() < limit+1){
            int value = myRandom(limit);
            value_move(value);
        }
        QFile file(filepath);
        if(file.open(QIODevice::WriteOnly)){
            QTextStream stream(&file);
            for(QMap<int,int>::iterator it=char_value_map.begin();it!=char_value_map.end();it++){
                stream<<it.key()<<" "<<it.value()<<'\n';
            }
            file.flush();
            file.close();
            LOGDBG("%s: %s",filepath,"create_map and file closed.");
        }
        else {
            LOGDBG("%s","file open failed.");
        }
    }
    else {
        LOGDBG("read create_map file");
        FILE *fp = fopen(filepath,"r");
        if(NULL == fp){
            LOGDBG("fopen %s failed.",filepath);
            return ;
        }
        int key;
        int value;
        while(EOF != fscanf(fp,"%d %d",&key, &value)){
            char_value_map[key] = value;
        }
        fclose(fp);
    }
}

void CharMap::read_map()
{
    char_value_map.clear();
    FILE *fp = fopen(filepath,"r");
    if(NULL == fp){
        LOGDBG("fopen %s failed: %s", strerror(errno));
        return ;
    }
    int key;
    int value;
    while(fscanf(fp,"%d%d",&key,&value) != EOF){
        char_value_map[key] = value;
    }
    fclose(fp);
}

void CharMap::value_move(int value)
{
    while(char_value_map.contains(value)){
        value++;
        if(value > limit) value = 0;
    }
    int count = char_value_map.size();
    char_value_map[value] = count;
}

QMap<int, int> CharMap::getMap()
{
    return this->char_value_map;
}
