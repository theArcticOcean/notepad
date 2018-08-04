#ifndef LOG_H
#define LOG_H

#include <stdarg.h>
#include <QDebug>
#include <stdio.h>
#include <unistd.h>

static QString logFilePath;

void LogInit();

void LOGBASE(const char *file, const int line, const char *funcName, const char * fmt, ...);

#define  LOGDBG(fmt,args...)   LOGBASE(__FILE__,__LINE__,__FUNCTION__, fmt, ##args)

#endif // LOG_H
