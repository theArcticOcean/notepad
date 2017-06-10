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

class textFind : public QWidget
{
    Q_OBJECT

public:
    explicit textFind(QWidget *parent = 0);
    QString getPattern();
    void setTextEdit(QTextEdit *textEdit);
    void search();
    void closeEvent(QCloseEvent *event);
//    void simNext();
//    void regNext();

private:
    QString pattern;

    QLabel *label;
    QLineEdit *line;
    QPushButton *findButton;
    //QPushButton *nextButton;
    QVBoxLayout *vLayout;
    QHBoxLayout *hLayout1;
    QHBoxLayout *hLayout2;
    QGroupBox *group1;
    //QGroupBox *group2;
    QRadioButton *simpleText;
    QRadioButton *regExp;
    //QRadioButton *upDirect;
    //QRadioButton *downDirect;

    QTextEdit *textEdit;

signals:

public slots:
    void setPattern();
    //void nextOne();
};

#endif // TEXTFIND_H
