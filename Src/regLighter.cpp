#include "./Inc/regLighter.h"
#include <QTextCharFormat>
#include <QColor>
#include <QRegularExpression>

regLighter::regLighter(QTextDocument * parent,QString pattern):
    QSyntaxHighlighter(parent)
{
    this->pattern = pattern;
}

void regLighter::highlightBlock(const QString &text)
{
    QTextCharFormat myClassFormat;
    QColor color(0,0,100,100);
    myClassFormat.setBackground(color); //Qt::darkBlue
    //myClassFormat.setFontWeight(QFont::Bold);
    //myClassFormat.setForeground(Qt::darkMagenta);
    QRegExp expression(pattern);
    int index = text.indexOf(expression);
    while (index >= 0) {
        int length = expression.matchedLength();
        setFormat(index, length, myClassFormat);
        index = text.indexOf(expression, index + length);
    }
}

