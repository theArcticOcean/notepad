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
#include <QSharedPointer>

#include "simlighter.h"

class TextReplace : public QWidget
{
    Q_OBJECT

public:
    explicit TextReplace(QWidget *parent = 0);
    ~TextReplace();
    QString getPattern();
    void setTextEdit(QTextEdit *textEdit);
    void closeEvent(QCloseEvent *event);
    void setTabWidget(QTabWidget *tabWidget);
    QString normalTitle(QString text);

private:
    QString pattern;

    QSharedPointer<QLabel> repLab, arrowLab;
    QSharedPointer<QLineEdit> fromText, toText;
    QSharedPointer<QLabel> regexp;
    QSharedPointer<QLabel> simple;
    QSharedPointer<QPushButton> regAll;
    QSharedPointer<QPushButton> simRepThis, simRepAll;
    QSharedPointer<QPushButton> simNextOne;
    QSpacerItem *spacer;        // add it to layout, layout would delete it.

    QSharedPointer<QVBoxLayout> vLayout;
    QSharedPointer<QHBoxLayout> hLayout1, hLayout2, hLayout3;
    QSharedPointer<QRadioButton> insensitive, sensitive;
    QSharedPointer<QTextCharFormat> charFormat;

    QTextEdit* textEdit;
    QTabWidget* tabWidget;
    QSharedPointer<SimLighter> highlighter;
    bool simRepFound;

signals:

public slots:
    void setPattern();
    void simRepThisFun();
    void simRepAllFun();
    void simSkip();
    void regRepAll();
};

#endif // TEXTREPLACE_H
