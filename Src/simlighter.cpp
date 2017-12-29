#include "./Inc/simlighter.h"
#include <QTextCursor>

simLighter::simLighter(QTextDocument *parent, QString pattern):
    QSyntaxHighlighter(parent)
{
    this->pattern = pattern;
}

void simLighter::highlightBlock(const QString &text)
{
    QTextCharFormat myClassFormat;
    QColor color(0,0,100,100);
    myClassFormat.setBackground(color); //Qt::darkBlue
    //myClassFormat.setFontWeight(QFont::Bold);
    //myClassFormat.setForeground(Qt::darkMagenta);
    int index = text.indexOf(pattern);
    while (index >= 0) {
        int length = pattern.length(); //expression.matchedLength();
        setFormat(index, length, myClassFormat);
        index = text.indexOf(pattern, index + length);
    }
}
