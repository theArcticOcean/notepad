#include <QTextCharFormat>
#include <QColor>
#include <QMessageBox>

#include "textfind.h"
#include "regLighter.h"
#include "simlighter.h"
#include "log.h"
#include "commondata.h"

textFind::textFind(QWidget *parent)
    : QWidget(parent)
    , highlighter( NULL )
{
    label = new QLabel("find :  ");
    line = new QLineEdit();
    findButton = new QPushButton("find");
    hLayout1 = new QHBoxLayout();
    hLayout1->addWidget(label);
    hLayout1->addWidget(line);
    hLayout1->addWidget(findButton);

    vLayout = new QVBoxLayout();
    vLayout->addLayout(hLayout1);

    group1 = new QGroupBox("find pattern: ");
    simpleText = new QRadioButton("simple text",group1);
    regExp = new QRadioButton("regular expression",group1);
    simpleText->setGeometry(QRect(0,35,92,21));
    regExp->setGeometry(QRect(0,65,92,21));

    hLayout2 = new QHBoxLayout();
    hLayout2->addWidget(group1);
    vLayout->addLayout(hLayout2);
    QIcon icon;
    icon.addFile(": /images/find.ico");
    this->setWindowIcon(icon);
    this->resize(400,180);
    this->setLayout(vLayout);
    this->setWindowTitle("find window");
    this->setWindowIcon(QIcon(ICON_PATH));

    connect(findButton,SIGNAL(clicked()),this,SLOT(setPattern()));
}

textFind::~textFind()
{
    LOGDBG( "~ textFind ~" );
//    if( NULL != highlighter )
//    {
//        delete highlighter;
//        highlighter = NULL;
//    }
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

    if(regExp->isChecked()){
        if( NULL != highlighter )
        {
            delete highlighter;
            highlighter = NULL;
        }
        highlighter = new regLighter(textEdit->document(),pattern);
    }
    else if(simpleText->isChecked())
    {
        if( NULL != highlighter )
        {
            delete highlighter;
            highlighter = NULL;
        }
        highlighter = new simLighter(textEdit->document(),pattern);
    }
    else
    {
        QMessageBox::information(this,"find pattern","please choose a find pattern.");
    }
}

void textFind::closeEvent(QCloseEvent *event)
{
    LOGDBG("%s","close event");
    // we can make sure that pattern match nothing by the following way.
    QString pattern = textEdit->toPlainText()+"hahhahaha";
    if( NULL != highlighter )
    {
        delete highlighter;
        highlighter = NULL;
    }
    highlighter = new regLighter(textEdit->document(),pattern);
    QWidget::closeEvent(event);
}

void textFind::setPattern()
{
    LOGDBG("you click find button");
    pattern = line->text();
    search();
}
