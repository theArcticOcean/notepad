#include "xmlrecorder.h"
#include <QDomComment>
#include <QDomElement>
#include <QTextEdit>
#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QXmlInputSource>
#include <QXmlSimpleReader>
#include <QDebug>
#include <QtCore>

xmlRecorder::xmlRecorder(QTabWidget *tabWidget)
{
    this->tabWidget = tabWidget;
    xmlPath = QCoreApplication::applicationDirPath()+"/backup/record.xml";
}

void xmlRecorder::writeXML()
{
    QDomDocument doc;
    QDomElement root = doc.createElement("root");
    doc.appendChild(root);
    qint8 tabs = tabWidget->count();
    for(qint8 i=0;i<tabs;i++){
        QString name = tabWidget->tabText(i);
        QDomElement file = doc.createElement("file");
        file.setAttribute("name",name);
        root.appendChild(file);
    }

    QFile recordFile(xmlPath);
    if(recordFile.open(QFile::WriteOnly | QFile::Text | QIODevice::Truncate)){
       QTextStream in(&recordFile);
       in<<doc.toString();
       recordFile.flush();
       recordFile.close();
       qDebug()<<"finished.";
   }
    else qDebug()<<"file open failed.";
}

void xmlRecorder::readXML()
{
    QFile file(xmlPath);
    qDebug()<<xmlPath;
    QDomDocument doc;
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        doc.setContent(&file);
        QDomElement element = doc.documentElement();
        qDebug()<<readNode(element);
        file.close();
    }
}

QString xmlRecorder::readNode(QDomElement element)
{
    QString text = "";
    for(QDomNode n = element.firstChild(); !n.isNull(); n = n.nextSibling())
    {
        QDomElement e = n.toElement();
        text = text + e.attribute("name")+"/";
        QString path = QCoreApplication::applicationDirPath();
        path = path+"/backup/"+e.attribute("name");
        QFile file(path);
        if(file.open(QFile::ReadOnly | QFile::Text)){
            QTextStream out(&file);
            QString context = out.readAll();
            QTextEdit *text = new QTextEdit();
            text->setPlainText(context);
            QIcon *icon = new QIcon(":/images/new.png");
            tabWidget->addTab(text,*icon,e.attribute("name"));
            tabWidget->setCurrentWidget(text);
            file.close();
        }
        else qDebug()<<"readnode file open failed:  "<<path;
    }
    return text;
}
