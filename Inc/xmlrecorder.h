#ifndef XMLRECORDER_H
#define XMLRECORDER_H

#include <QTabWidget>
#include <QDomElement>

class xmlRecorder
{
    QTabWidget *tabWidget;
    QString xmlPath;
public:
    xmlRecorder(QTabWidget *tabWidget);
    void writeXML();
    void readXML();
    QString readNode(QDomElement element);
};

#endif // XMLRECORDER_H
