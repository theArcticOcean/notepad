#include "charlocker.h"
#include <QFile>
#include <QDataStream>
#include <QTextStream>
#include <QDebug>
#include <QTextCodec>

charLocker::charLocker(QMap<int, int> charMap)
{
    this->char_value_map = char_value_map;
    QTextCodec *textCodec = QTextCodec::codecForLocale();
    textCodec->setCodecForLocale(textCodec);
}

void charLocker::encrypt()
{
    QFile file(textPath);
    if(!file.open(QFile::ReadOnly)){
        qDebug()<<"encrypt file read open failed.";
        return ;
    }
    bytes = file.readAll();
    for(int i=0;i<bytes.size();i++){
        int key;
        int value ;

        key = bytes[i]+128;
        value = char_value_map[key];
        bytes[i] = value-128;
    }
    file.close();

    if(!file.open(QFile::WriteOnly)){
        qDebug()<<"encrypt write file open failed.";
        return ;
    }
    //QDataStream stream(&file);
    QTextStream stream(&file);
    stream<<bytes;
    file.flush();
    file.close();
    qDebug()<<"write encrypted context to file successfully.";
    //system("cat ./txt");
}

void charLocker::decrypt()
{
    QFile file(textPath);
    if(!file.open(QFile::WriteOnly)){
        qDebug()<<"decrypt file write open failed.";
        return ;
    }
    for(int i=0;i<bytes.size();i++){
        int key;
        int value;

        value = bytes[i]+128;
        key = char_value_map.key(value);
        bytes[i] = key-128;
    }
    //QDataStream stream(&file);
    QTextStream stream(&file);
    stream<<bytes;
    file.flush();
    file.close();
    //system("cat ./txt");
    //qDebug()<<bytes;
}
