#include "./Inc/handler.h"
#include <QStringList>
#include <stdio.h>
#include <QtDebug>

QString baseName(QString str)
{
    QStringList strlist = str.split("/");
    return strlist.last();
}


char *bytesRead(const char *path, int *byte_number)
{
    int ret;
    FILE *fp = fopen(path,"r");
    fseek(fp,0,SEEK_END);
    long pos_end = ftell(fp);
    fseek(fp,0,SEEK_SET);
    long pos = ftell(fp);
    *byte_number = pos_end - pos;
    char *bytes = (char *)malloc(sizeof(char)*(*byte_number));
    memset(bytes,0,*byte_number);
    int i = 0;
    while(pos < pos_end){
        ret = fscanf(fp,"%c",&bytes[i]);
        if(ret <= 0){
            qDebug()<<"fscanf bytesRead error ";
            return NULL;
        }
        pos = ftell(fp);
        i++;
    }
    fclose(fp);
    return bytes;
}


void bytesWrite(const char *path, const char *bytes, const int byte_number)
{
    int ret;
    FILE *fp = fopen(path,"w");
    int i;
    for(i=0;i<byte_number;i++){
        ret = fprintf(fp,"%c",bytes[i]);
        if(ret <= 0){
            qDebug()<<"fprintf bytesWrite error ";
            return ;
        }
    }
    fclose(fp);
}
