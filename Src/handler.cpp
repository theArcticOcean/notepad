#include <QStringList>
#include <stdio.h>
#include <QtDebug>
#include <QMutex>
#include <fcntl.h>
#include <unistd.h>

#include "notepad.h"
#include "handler.h"
#include "log.h"

QMutex notePad::mutexForRW;

QString baseName(QString str)
{
    QStringList strlist = str.split("/");
    return strlist.last();
}

char *bytesRead(const char *path, int *byte_number)
{
    notePad::mutexForRW.lock();
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
            LOGDBG("%s","fscanf bytesRead error");
            return NULL;
        }
        pos = ftell(fp);
        i++;
    }
    fclose(fp);
    LOGDBG("path %s read %d bytes: %s",path,*byte_number,bytes);
    notePad::mutexForRW.unlock();
    return bytes;
}


void bytesWrite(const char *path, const char *bytes, const int byte_number)
{
    notePad::mutexForRW.lock();
    int ret;
#ifdef Q_OS_LINUX
    int fd = open(path,O_WRONLY | O_CREAT | O_TRUNC, 0666);
#else
    int fd = open(path,O_WRONLY | O_CREAT | O_TRUNC);
#endif
    if(fd == NULL){
        LOGDBG("fopen failed.");
        return ;
    }
    LOGDBG("path: %s, write %d bytes: %s",path, byte_number, bytes);
    ret = write(fd,bytes,byte_number);
    if(ret <= 0){
        LOGDBG("write %s failed", path);
        close(fd);
        goto end;
    }
end:
    close(fd);
    notePad::mutexForRW.unlock();
}
