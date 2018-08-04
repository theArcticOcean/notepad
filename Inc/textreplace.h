#ifndef TEXTREPLACE_H
#define TEXTREPLACE_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QTextEdit>
#include <QDebug>
#include <QCloseEvent>
#include <QSpacerItem>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QTabWidget>
#include "Inc/simlighter.h"

class textReplace : public QWidget
{
    Q_OBJECT

public:
    explicit textReplace(QWidget *parent = 0);
    ~textReplace();
    QString getPattern();
    void setTextEdit(QTextEdit *textEdit);
    void closeEvent(QCloseEvent *event);
    void setTabWidget(QTabWidget *tabWidget);
    static QString normalTitle(QString text);

private:
    QString pattern;

    QLabel *repLab, *arrowLab;
    QLineEdit *fromText, *toText;
    QLabel *regexp;
    QLabel *simple;
    QPushButton *regAll;
    QSpacerItem *spacer;
    QPushButton *simRepThis, *simRepAll;
    QPushButton *simNextOne;

    QVBoxLayout *vLayout;
    QHBoxLayout *hLayout1;
    QHBoxLayout *hLayout2;
    QHBoxLayout *hLayout3;
    QRadioButton *insensitive;
    QRadioButton *sensitive;
    QTextCharFormat *charFormat;

    QTextEdit *textEdit;
    QTabWidget *tabWidget;
    bool simRepFound;
    simLighter *highlighter;

signals:

public slots:
    void setPattern();
    void simRepThisFun();
    void simRepAllFun();
    void simSkip();
    void regRepAll();
};

#endif // TEXTREPLACE_H
