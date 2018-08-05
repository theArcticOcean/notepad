#ifndef TEXTFIND_H
#define TEXTFIND_H

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
#include <QSharedPointer>

#include "reglighter.h"
#include "simlighter.h"

enum HightLighterType{ NoType, regLighterType, simLighterType };

class TextFind : public QWidget
{
    Q_OBJECT

public:
    explicit TextFind(QWidget *parent = 0);
    ~TextFind();
    QString getPattern();
    void setTextEdit(QTextEdit *textEdit);
    void search();
    void closeEvent(QCloseEvent *event);

private:
    void deleteHighLighterPtr();

    QString pattern;

    QSharedPointer<QLabel> label;
    QSharedPointer<QLineEdit> line;
    QSharedPointer<QPushButton> findButton;
    QSharedPointer<QVBoxLayout> vLayout;
    QSharedPointer<QHBoxLayout> hLayout1, hLayout2;
    QSharedPointer<QGroupBox> group1;
    QSharedPointer<QRadioButton> simpleText, regExp;

    QTextEdit *textEdit;
    void *highlighter;
    HightLighterType lighterType;

signals:

public slots:
    void setPattern();
};

#endif // TEXTFIND_H
