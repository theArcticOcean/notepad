#include "textfind.h"
#include "regLighter.h"
#include "simlighter.h"
#include <QTextCharFormat>
#include <QColor>
#include <QMessageBox>

textFind::textFind(QWidget *parent):
    QWidget(parent)
{
    label = new QLabel("find :  ");
    line = new QLineEdit();
    findButton = new QPushButton("find");
    //nextButton = new QPushButton("next one");
    hLayout1 = new QHBoxLayout();
    hLayout1->addWidget(label);
    hLayout1->addWidget(line);
    hLayout1->addWidget(findButton);
    //hLayout1->addWidget(nextButton);

    vLayout = new QVBoxLayout();
    vLayout->addLayout(hLayout1);
    //vLayout->addChildLayout(hLayout1);

    group1 = new QGroupBox("find pattern: ");
    simpleText = new QRadioButton("simple text",group1);
    regExp = new QRadioButton("regular expression",group1);
    simpleText->setGeometry(QRect(0,35,92,21));
    regExp->setGeometry(QRect(0,65,92,21));

//    group2 = new QGroupBox("direction:");
//    upDirect = new QRadioButton("up",group2);
//    downDirect = new QRadioButton("down",group2);
//    //Rect's (x,y,w,h) based on parent widget.
//    upDirect->setGeometry(QRect(0,35,92,21));
//    downDirect->setGeometry(QRect(0,65,92,21));

    hLayout2 = new QHBoxLayout();
    hLayout2->addWidget(group1);
    //hLayout2->addWidget(group2);
    vLayout->addLayout(hLayout2);
    QIcon icon;
    icon.addFile(":/images/find.ico");
    this->setWindowIcon(icon);
    this->resize(400,180);
    this->setLayout(vLayout);
    this->setWindowTitle("find window");

    connect(findButton,SIGNAL(clicked()),this,SLOT(setPattern()));
//    connect(nextButton,SIGNAL(clicked()),this,SLOT(nextOne()));
}

QString textFind::getPattern()
{
    return pattern;
}

void textFind::setTextEdit(QTextEdit *textEdit)
{
    this->textEdit = textEdit;
}

void textFind::search()
{
    if(pattern.isEmpty()) {
        QMessageBox::warning(this,"find context","please enter your context to find.");
        return ;
    }

    if(regExp->isChecked()) regLighter *highlighter = \
            new regLighter(textEdit->document(),pattern);
    else if(simpleText->isChecked()) simLighter *highlighter = \
            new simLighter(textEdit->document(),pattern);
    else QMessageBox::information(this,"find pattern","please choose a find pattern.");
}

void textFind::closeEvent(QCloseEvent *event)
{
    qDebug()<<"close event";
    // we can make sure that pattern match nothing by the following way.
    QString pattern = textEdit->toPlainText()+"hahhahaha";
    regLighter *highlighter = new regLighter(textEdit->document(),pattern);
    QWidget::closeEvent(event);
}

void textFind::setPattern()
{
    pattern = line->text();
    search();
}
