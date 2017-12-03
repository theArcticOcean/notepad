#include "./Inc/charmap.h"
#include <stdlib.h>
#include <time.h>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <stdio.h>

charMap::charMap(char *str):filepath(str)
{
    srand(time(NULL));
    limit = (1<<8)-1;
}

int charMap::myRandom(int limit)
{
    return int((double)random()/RAND_MAX*limit+0.5);
}

void charMap::create_map()
{
    while(char_value_map.size() < limit+1){
        int value = myRandom(limit);
        value_move(value);
    }
    QFile file(filepath);
    if(file.open(QIODevice::WriteOnly)){
        QTextStream stream(&file);
        for(QMap<int,int>::iterator it=char_value_map.begin();it!=char_value_map.end();it++){
            stream<<it.key()<<" "<<it.value()<<'\n';
            //qDebug()<<it.key()<<" "<<it.value();
        }
        file.flush();
        file.close();
        qDebug()<<"create_map and file closed.";
    }
    else {
        qDebug()<<"file open failed.";
    }
}

void charMap::read_map()
{
    char_value_map.clear();
    FILE *fp = fopen(filepath,"r");
    int key;
    int value;

    while(fscanf(fp,"%d%d",&key,&value) != EOF){
        char_value_map[key] = value;
        //printf("%d %d\n",key,value);
    }
//    QFile file(filepath);
//    qDebug()<<filepath;
//    system("cat ./folder/.map");  /* test here */
//    getchar();
//    if(file.open(QIODevice::ReadOnly)){
//        QTextStream in(&file);
//        signed int value, key;
//        while(!in.atEnd()){
//            in>>key>>value;
//            char_value_map[key] = value;
//        }
//        file.close();
//    }
//    else {
//        qDebug()<<"read map file open failed.";
//        return ;
//    }

//    QMapIterator<int,int> it(char_value_map);
//    while(it.hasNext()){
//        it.next();
//        qDebug()<<it.key()<<" "<<it.value();
//    }
//    //system("cat ./map.txt");

}

void charMap::value_move(int value)
{
    while(char_value_map.contains(value)){
        value++;
        if(value > limit) value = 0;
    }
    int count = char_value_map.size();
    char_value_map[value] = count;
}

QMap<int, int> charMap::getMap()
{
    return this->char_value_map;
}
