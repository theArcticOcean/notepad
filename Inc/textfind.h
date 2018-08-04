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

#include "regLighter.h"
#include "simlighter.h"

class textFind : public QWidget
{
    Q_OBJECT

public:
    explicit textFind(QWidget *parent = 0);
    ~textFind();
    QString getPattern();
    void setTextEdit(QTextEdit *textEdit);
    void search();
    void closeEvent(QCloseEvent *event);

private:
    QString pattern;

    QLabel *label;
    QLineEdit *line;
    QPushButton *findButton;
    QVBoxLayout *vLayout;
    QHBoxLayout *hLayout1;
    QHBoxLayout *hLayout2;
    QGroupBox *group1;
    QRadioButton *simpleText;
    QRadioButton *regExp;

    QTextEdit *textEdit;
    void *highlighter;
signals:

public slots:
    void setPattern();
};

#endif // TEXTFIND_H
