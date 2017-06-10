#ifndef regLighter_H
#define regLighter_H

#include <QSyntaxHighlighter>
#include <QString>
#include <QTextEdit>
#include <QDebug>

class regLighter : public QSyntaxHighlighter
{
    QString pattern;
public:
    regLighter(QTextDocument * parent,QString pattern);
    void highlightBlock(const QString &text);
};

#endif // regLighter_H
