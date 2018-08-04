
#include "./Inc/textreplace.h"
#include "./Inc/log.h"
#include <QMessageBox>
#include "./Inc/regLighter.h"
#include "./Inc/simlighter.h"
#include "Inc/commondata.h"
#include <QRegExp>
#include <QCoreApplication>

textReplace::textReplace(QWidget *parent)
    : QWidget(parent)
    , highlighter ( NULL )
{
    QString title;
    title = normalTitle("replace: ");
    simRepFound = false;
    repLab = new QLabel (title);
    arrowLab = new QLabel (" ==> ");
    fromText = new QLineEdit();
    toText = new QLineEdit();
    hLayout1 = new QHBoxLayout();
    hLayout1->addWidget(repLab);
    hLayout1->addWidget(fromText);
    hLayout1->addWidget(arrowLab);
    hLayout1->addWidget(toText);

    insensitive = new QRadioButton("insensitive");
    sensitive = new QRadioButton("sensitive");
    insensitive->setGeometry(QRect(0,25,92,21));
    sensitive->setGeometry(QRect(0,55,92,21));

    title = normalTitle("regular expression replace:");
    regexp = new QLabel(title);
    regAll = new QPushButton("replace all");
    spacer = new QSpacerItem(this->width()/3,21);
    hLayout2 = new QHBoxLayout();
    hLayout2->addWidget(regAll);
    hLayout2->addSpacerItem(spacer);

    title = normalTitle("simple pattern replace:");
    simple = new QLabel(title);
    simNextOne = new QPushButton("skip");
    simRepThis = new QPushButton("replace this");
    simRepAll = new QPushButton("replace all");
    hLayout3 = new QHBoxLayout();
    hLayout3->addWidget(simNextOne);
    hLayout3->addWidget(simRepThis);
    hLayout3->addWidget(simRepAll);

    vLayout = new QVBoxLayout();
    vLayout->addLayout(hLayout1);
    vLayout->addWidget(regexp);
    vLayout->addLayout(hLayout2);
    vLayout->addWidget(simple);
    vLayout->addWidget(insensitive);
    vLayout->addWidget(sensitive);
    vLayout->addLayout(hLayout3);

    QIcon icon;
    icon.addFile(":/images/replace.ico");
    this->setLayout(vLayout);
    this->setWindowTitle("replace window");
    this->setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint); // maxBnt 按位取反
    this->setWindowIcon(QIcon(ICON_PATH));

    charFormat = new QTextCharFormat();
    QColor color(100,0,100,100);
    charFormat->setBackground(color);

    connect(simRepThis,SIGNAL(clicked()),this,SLOT(simRepThisFun()));
    connect(simRepAll,SIGNAL(clicked()),this,SLOT(simRepAllFun()));
    connect(simNextOne,SIGNAL(clicked()),this,SLOT(simSkip()));
    connect(regAll,SIGNAL(clicked()),this,SLOT(regRepAll()));
}

textReplace::~textReplace()
{
    LOGDBG( "~ textReplace ~" );
}

QString textReplace::getPattern()
{
    return pattern;
}

void textReplace::setTextEdit(QTextEdit *textEdit)
{
    this->textEdit = textEdit;
}

void textReplace::simSkip()
{
    QString from = fromText->text();
    if(from.isEmpty()){
        QMessageBox::warning(this,tr("attention"),\
                             tr("please enter replace words."),\
                             QMessageBox::Ok);
        return ;
    }
    highlighter = new simLighter(textEdit->document(),from);

    QString to = toText->text();
    bool sensi = false;
    if(sensitive->isChecked()) sensi = true;
    else if(!insensitive->isChecked() && !sensitive->isChecked()){
        QMessageBox::warning(this,tr("attention"),\
                             tr("please choose case sensitivity"),\
                             QMessageBox::Ok);
        return ;
    }

    QTextCursor textCursor = textEdit->textCursor();
    if(sensi == false){
        // find move cursor
        if(textEdit->find(from)){
            // the following sentence can make words which marched highlight
            textCursor = textEdit->textCursor();
            simRepFound = true;
        }
        else {
            textEdit->moveCursor(QTextCursor::Start);
        }
    }
    else {
        if(textEdit->find(from,QTextDocument::FindCaseSensitively)){
            // the following sentence can make words which marched highlight
            textCursor = textEdit->textCursor();
            simRepFound = true;
        }
        else {
            textEdit->moveCursor(QTextCursor::Start);
        }
    }
}

void textReplace::simRepAllFun()
{
    QString from = fromText->text();
    if(from.isEmpty()){
        QMessageBox::warning(this,tr("attention"),\
                             tr("please enter replace words."),\
                             QMessageBox::Ok);
        return ;
    }
    highlighter = new simLighter(textEdit->document(),from);

    QString to = toText->text();
    bool sensi = false;
    if(sensitive->isChecked()) sensi = true;
    else if(!insensitive->isChecked() && !sensitive->isChecked()){
        QMessageBox::warning(this,tr("attention"),\
                             tr("please choose case sensitivity"),\
                             QMessageBox::Ok);
        return ;
    }


    QTextCursor textCursor = textEdit->textCursor();
    textCursor.movePosition(QTextCursor::Start);
    textEdit->setTextCursor(textCursor);
    if(sensi == false){
        while(textEdit->find(from)){
            textCursor = textEdit->textCursor();
            // replace words
            textCursor.insertText(to);
        }
    }
    else {
        while(textEdit->find(from,QTextDocument::FindCaseSensitively)){
            textCursor = textEdit->textCursor();
            textCursor.insertText(to);
        }
    }
}

void textReplace::simRepThisFun()
{
    QString from = fromText->text();
    if(from.isEmpty()){
        QMessageBox::warning(this,tr("attention"),\
                             tr("please enter replace words."),\
                             QMessageBox::Ok);
        return ;
    }

    bool sensi = false;
    if(sensitive->isChecked()) sensi = true;
    else if(!insensitive->isChecked() && !sensitive->isChecked()){
        QMessageBox::warning(this,tr("attention"),\
                             tr("please choose case sensitivity"),\
                             QMessageBox::Ok);
        return ;
    }

    QString to = toText->text();
    QTextCursor textCursor = textEdit->textCursor();
    if( simRepFound )
    {
        textCursor.insertText(to);
        simRepFound = false;
    }
    LOGDBG("pos: %d",textCursor.position());
}

void textReplace::regRepAll()
{
    QString from = fromText->text();
    if(from.isEmpty()){
        QMessageBox::warning(this,tr("attention"),\
                             tr("please enter replace words."),\
                             QMessageBox::Ok);
        return ;
    }

    QString newText = "";
    QTextEdit *curEdit = static_cast<QTextEdit *>(tabWidget->currentWidget());
    QString Text = curEdit->toPlainText();
    QStringList list = Text.split('\n');
    foreach (QString line, list) {
        line = line.replace(QRegExp(fromText->text()),toText->text());
        newText = newText+line+"\n";
    }
    curEdit->setPlainText(newText);
}

void textReplace::closeEvent(QCloseEvent *event)
{
    LOGDBG("%s","close event");
    // we can make sure that pattern match nothing by the following way.
    QString pattern = textEdit->toPlainText()+"hahhahaha";
    highlighter = new simLighter(textEdit->document(), pattern);
    if( NULL != highlighter )
    {
        delete highlighter;
        highlighter = NULL;
    }
    QWidget::closeEvent(event);
}

void textReplace::setTabWidget(QTabWidget *tabWidget)
{
    this->tabWidget = tabWidget;
}

QString textReplace::normalTitle(QString text)
{
    //    background:#DDDD00;  // the back color is yellow
    QString str = "<font style=\"color:#000000\" size=\"4\">";
    str = str+text;
    str = str+"</font>";
    return str;
}

void textReplace::setPattern()
{
    pattern = fromText->text();
}
