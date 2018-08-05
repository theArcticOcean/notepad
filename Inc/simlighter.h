#ifndef SIMLIGHTER_H
#define SIMLIGHTER_H

#include <QSyntaxHighlighter>
#include <QString>
#include <QTextEdit>
#include <QDebug>
#include <QTextEdit>

class SimLighter : public QSyntaxHighlighter
{   
public:
    SimLighter(QTextDocument * parent,QString pattern);
    void highlightBlock(const QString &text);

private:
    QString pattern;
};

#endif // SIMLIGHTER_H
