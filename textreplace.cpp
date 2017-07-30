#include "textreplace.h"
#include <QMessageBox>
#include "regLighter.h"
#include "simlighter.h"
#include <QRegExp>
#include <QCoreApplication>

textReplace::textReplace(QWidget *parent) :
    QWidget(parent)
{
    repLab = new QLabel ("replace: ");
    arrowLab = new QLabel ("-->");
    fromText = new QLineEdit();
    toText = new QLineEdit();
    hLayout1 = new QHBoxLayout();
    hLayout1->addWidget(repLab);
    hLayout1->addWidget(fromText);
    hLayout1->addWidget(arrowLab);
    hLayout1->addWidget(toText);

    group1 = new QGroupBox("case sensitivity: ");
    insensitive = new QRadioButton("insensitive",group1);
    sensitive = new QRadioButton("sensitive",group1);
//    insensitive->setGeometry(QRect(0,25,92,21));
//    sensitive->setGeometry(QRect(0,55,92,21));

    regexp = new QLabel("<font style=\"background:#DDDD00; color:#000000\" size=\"4\">regular expression replace:</font>");
    regAll = new QPushButton("replace all");
    spacer = new QSpacerItem(this->width()/3,21);
    hLayout2 = new QHBoxLayout();
    hLayout2->addWidget(regAll);
    hLayout2->addSpacerItem(spacer);

    simple = new QLabel("<font style=\"background:#DDDD00; color:#000000\" size=\"4\">simple pattern replace:</font>");
    simRepThis = new QPushButton("replace this");
    simRepAll = new QPushButton("replace all");
    simNextOne = new QPushButton("skip");
    hLayout3 = new QHBoxLayout();
    hLayout3->addWidget(simRepThis);
    hLayout3->addWidget(simRepAll);
    hLayout3->addWidget(simNextOne);

    vLayout = new QVBoxLayout();
    vLayout->addLayout(hLayout1);
    vLayout->addWidget(regexp);
    vLayout->addLayout(hLayout2);
    vLayout->addWidget(simple);
    vLayout->addWidget(group1);
    vLayout->addWidget(insensitive);
    vLayout->addWidget(sensitive);
    vLayout->addLayout(hLayout3);

    this->setLayout(vLayout);
    this->setWindowTitle("replace window");
    QIcon icon;
    icon.addFile(":/images/replace.ico");
    this->setWindowIcon(icon);

    charFormat = new QTextCharFormat();
    QColor color(100,0,100,100);
    charFormat->setBackground(color);

    connect(simRepThis,SIGNAL(clicked()),this,SLOT(simRepThisFun()));
    connect(simRepAll,SIGNAL(clicked()),this,SLOT(simRepAllFun()));
    connect(simNextOne,SIGNAL(clicked()),this,SLOT(simSkip()));
    connect(regAll,SIGNAL(clicked()),this,SLOT(regRepAll()));
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
                             tr("please enter search words."),\
                             QMessageBox::Ok);
        return ;
    }
    simLighter *highlighter = new simLighter(textEdit->document(),from);

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
        }
        else {
            QMessageBox::warning(this,tr("attention"),\
                                 tr("not found, maybe words are in front of cursor !"),\
                                 QMessageBox::Ok);
        }
    }
    else {
        if(textEdit->find(from,QTextDocument::FindCaseSensitively)){
            // the following sentence can make words which marched highlight
            textCursor = textEdit->textCursor();
        }
        else {
            QMessageBox::warning(this,tr("attention"),\
                                 tr("not found, maybe words are in front of cursor !"),\
                                 QMessageBox::Ok);
        }
    }
}

void textReplace::simRepAllFun()
{
    QString from = fromText->text();
    if(from.isEmpty()){
        QMessageBox::warning(this,tr("attention"),\
                             tr("please enter search words."),\
                             QMessageBox::Ok);
        return ;
    }
    simLighter *highlighter = new simLighter(textEdit->document(),from);

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
    QTextCursor textCursor = textEdit->textCursor();
    QString to = toText->text();
    textCursor.insertText(to);
}

void textReplace::regRepAll()
{
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
    qDebug()<<"close event";
    // we can make sure that pattern match nothing by the following way.
    QString pattern = textEdit->toPlainText()+"hahhahaha";
    regLighter *highlighter = new regLighter(textEdit->document(),pattern);
    QWidget::closeEvent(event);
}

void textReplace::setTabWidget(QTabWidget *tabWidget)
{
    this->tabWidget = tabWidget;
}

void textReplace::setPattern()
{
    pattern = fromText->text();
}