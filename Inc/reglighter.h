#ifndef regLighter_H
#define regLighter_H

#include <QSyntaxHighlighter>
#include <QString>
#include <QTextEdit>
#include <QDebug>

class RegLighter : public QSyntaxHighlighter
{
public:
    RegLighter(QTextDocument * parent,QString pattern);
    void highlightBlock(const QString &text);

private:
    QString pattern;
};

#endif // regLighter_H
