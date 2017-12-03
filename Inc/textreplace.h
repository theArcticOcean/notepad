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

class textReplace : public QWidget
{
    Q_OBJECT

public:
    explicit textReplace(QWidget *parent = 0);
    QString getPattern();
    void setTextEdit(QTextEdit *textEdit);
    void closeEvent(QCloseEvent *event);
    void setTabWidget(QTabWidget *tabWidget);

private:
    QString pattern;

    QLabel *repLab, *arrowLab;
    QLineEdit *fromText, *toText;
    QLabel *simple;
    QLabel *regexp;
    QPushButton *regAll;
    QSpacerItem *spacer;
    QPushButton *simRepThis, *simRepAll;
    QPushButton *simNextOne;

    QVBoxLayout *vLayout;
    QHBoxLayout *hLayout1;
    QHBoxLayout *hLayout2;
    QHBoxLayout *hLayout3;
    QGroupBox *group1;
    QRadioButton *insensitive;
    QRadioButton *sensitive;
    QTextCharFormat *charFormat;

    QTextEdit *textEdit;
    QTabWidget *tabWidget;

signals:

public slots:
    void setPattern();
    void simRepThisFun();
    void simRepAllFun();
    void simSkip();
    void regRepAll();
};

#endif // TEXTREPLACE_H
