#ifndef SIMLIGHTER_H
#define SIMLIGHTER_H

#include <QSyntaxHighlighter>
#include <QString>
#include <QTextEdit>
#include <QDebug>
#include <QTextEdit>

class simLighter : public QSyntaxHighlighter
{
    QString pattern;
public:
    simLighter(QTextDocument * parent,QString pattern);
    void highlightBlock(const QString &text);
};

#endif // SIMLIGHTER_H
