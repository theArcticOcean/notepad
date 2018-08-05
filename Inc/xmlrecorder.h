#ifndef XMLRECORDER_H
#define XMLRECORDER_H

#include <QTabWidget>
#include <QDomElement>

class XmlRecorder
{
public:
    XmlRecorder(QTabWidget *tabWidget);
    void writeXML();
    void readXML();
    QString readNode(QDomElement element);

private:
    QTabWidget *tabWidget;
    QString xmlPath;

};

#endif // XMLRECORDER_H
