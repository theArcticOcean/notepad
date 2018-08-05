#include <QMessageBox>
#include <QRegExp>
#include <QCoreApplication>

#include "textreplace.h"
#include "log.h"
#include "reglighter.h"
#include "simlighter.h"
#include "commondata.h"

TextReplace::TextReplace( QWidget *parent )
    : QWidget( parent )
    , textEdit( NULL )
    , tabWidget( NULL )
{
    QString title;
    title = normalTitle("replace: ");
    simRepFound = false;
    repLab = QSharedPointer<QLabel>( new QLabel( title ) );
    arrowLab = QSharedPointer<QLabel>( new QLabel (" ==> ") );
    fromText = QSharedPointer<QLineEdit>( new QLineEdit() );
    toText = QSharedPointer<QLineEdit>( new QLineEdit() );
    hLayout1 = QSharedPointer<QHBoxLayout>( new QHBoxLayout() );
    hLayout1->addWidget( repLab.data() );
    hLayout1->addWidget( fromText.data() );
    hLayout1->addWidget( arrowLab.data() );
    hLayout1->addWidget( toText.data() );

    insensitive = QSharedPointer<QRadioButton>( new QRadioButton("insensitive") );
    sensitive = QSharedPointer<QRadioButton>( new QRadioButton("sensitive") );
    insensitive->setGeometry( QRect(0,25,92,21) );
    sensitive->setGeometry( QRect(0,55,92,21) );

    title = normalTitle("regular expression replace:");
    regexp =  QSharedPointer<QLabel>( new QLabel( title ) );
    regAll = QSharedPointer<QPushButton>( new QPushButton( "replace all" ) );
    spacer = new QSpacerItem( this->width() / 3, 21 );
    hLayout2 = QSharedPointer<QHBoxLayout>( new QHBoxLayout() );
    hLayout2->addWidget( regAll.data() );
    hLayout2->addSpacerItem( spacer );

    title = normalTitle("simple pattern replace:");
    simple = QSharedPointer<QLabel>( new QLabel( title ) );
    simNextOne = QSharedPointer<QPushButton>( new QPushButton( "skip" ) );
    simRepThis = QSharedPointer<QPushButton>( new QPushButton( "replace this" ) );
    simRepAll = QSharedPointer<QPushButton>( new QPushButton( "replace all" ) );
    hLayout3 = QSharedPointer<QHBoxLayout>( new QHBoxLayout() );
    hLayout3->addWidget( simNextOne.data() );
    hLayout3->addWidget( simRepThis.data() );
    hLayout3->addWidget( simRepAll.data() );

    vLayout = QSharedPointer<QVBoxLayout>( new QVBoxLayout() );
    vLayout->addLayout( hLayout1.data() );
    vLayout->addWidget( regexp.data() );
    vLayout->addLayout( hLayout2.data() );
    vLayout->addWidget( simple.data() );
    vLayout->addWidget( insensitive.data() );
    vLayout->addWidget( sensitive.data() );
    vLayout->addLayout( hLayout3.data() );

    QIcon icon;
    icon.addFile(":/images/replace.ico");
    this->setLayout( vLayout.data() );
    this->setWindowTitle("replace window");
    this->setWindowFlags( windowFlags()& ~Qt::WindowMaximizeButtonHint ); // maxBnt 按位取反
    this->setWindowIcon( QIcon( ICON_PATH ) );

    charFormat = QSharedPointer<QTextCharFormat>( new QTextCharFormat() );
    QColor color(100,0,100,100);
    charFormat->setBackground( color );

    connect( simRepThis.data(), SIGNAL( clicked() ), this, SLOT( simRepThisFun() ) );
    connect( simRepAll.data(), SIGNAL( clicked() ), this, SLOT( simRepAllFun() ) );
    connect( simNextOne.data(), SIGNAL( clicked() ), this, SLOT( simSkip() ) );
    connect( regAll.data(), SIGNAL( clicked() ), this, SLOT( regRepAll() ) );

    LOGDBG("TextReplace() finished.");
}

TextReplace::~TextReplace()
{
    textEdit = NULL;
    tabWidget = NULL;
    LOGDBG("~TextReplace() finished.");
}

QString TextReplace::getPattern()
{
    return pattern;
}

void TextReplace::setTextEdit( QTextEdit *textEdit )
{
    this->textEdit = textEdit;
}

void TextReplace::simSkip()
{
    QString from = fromText->text();
    if( from.isEmpty() ){
        QMessageBox::warning( this, tr("attention"),\
                             tr("please enter replace words."),\
                             QMessageBox::Ok );
        return ;
    }
    highlighter =  QSharedPointer<SimLighter>( new SimLighter( textEdit->document(), from ) );

    QString to = toText->text();
    bool sensi = false;
    if( sensitive->isChecked() ) sensi = true;
    else if(!insensitive->isChecked() && !sensitive->isChecked() ){
        QMessageBox::warning( this, tr("attention"),\
                             tr("please choose case sensitivity"),\
                             QMessageBox::Ok );
        return ;
    }

    QTextCursor textCursor = textEdit->textCursor();
    if( sensi == false ){
        // find move cursor
        if( textEdit->find( from ) ){
            // the following sentence can make words which marched highlight
            textCursor = textEdit->textCursor();
            simRepFound = true;
        }
        else {
            textEdit->moveCursor( QTextCursor::Start );
        }
    }
    else {
        if( textEdit->find( from, QTextDocument::FindCaseSensitively ) ){
            // the following sentence can make words which marched highlight
            textCursor = textEdit->textCursor();
            simRepFound = true;
        }
        else {
            textEdit->moveCursor( QTextCursor::Start );
        }
    }
}

void TextReplace::simRepAllFun()
{
    QString from = fromText->text();
    if( from.isEmpty() ){
        QMessageBox::warning( this, tr("attention"),\
                             tr("please enter replace words."),\
                             QMessageBox::Ok );
        return ;
    }
    highlighter = QSharedPointer<SimLighter>( new SimLighter( textEdit->document(), from ) );

    QString to = toText->text();
    bool sensi = false;
    if( sensitive->isChecked() ) sensi = true;
    else if(!insensitive->isChecked() && !sensitive->isChecked() ){
        QMessageBox::warning( this, tr("attention"),\
                             tr("please choose case sensitivity"),\
                             QMessageBox::Ok );
        return ;
    }


    QTextCursor textCursor = textEdit->textCursor();
    textCursor.movePosition( QTextCursor::Start );
    textEdit->setTextCursor( textCursor );
    if( sensi == false ){
        while( textEdit->find( from ) ){
            textCursor = textEdit->textCursor();
            // replace words
            textCursor.insertText( to );
        }
    }
    else {
        while( textEdit->find( from, QTextDocument::FindCaseSensitively ) ){
            textCursor = textEdit->textCursor();
            textCursor.insertText( to );
        }
    }
}

void TextReplace::simRepThisFun()
{
    QString from = fromText->text();
    if( from.isEmpty() ){
        QMessageBox::warning( this, tr("attention"),\
                             tr("please enter replace words."),\
                             QMessageBox::Ok );
        return ;
    }

    bool sensi = false;
    if( sensitive->isChecked() ) sensi = true;
    else if(!insensitive->isChecked() && !sensitive->isChecked() ){
        QMessageBox::warning( this, tr("attention"),\
                             tr("please choose case sensitivity"),\
                             QMessageBox::Ok );
        return ;
    }

    QString to = toText->text();
    QTextCursor textCursor = textEdit->textCursor();
    if( simRepFound )
    {
        textCursor.insertText( to );
        simRepFound = false;
    }
    LOGDBG("pos: %d", textCursor.position() );
}

void TextReplace::regRepAll()
{
    QString from = fromText->text();
    if( from.isEmpty() ){
        QMessageBox::warning( this, tr("attention"),\
                             tr("please enter replace words."),\
                             QMessageBox::Ok );
        return ;
    }

    QString newText = "";
    QTextEdit *curEdit = static_cast<QTextEdit *>( tabWidget->currentWidget() );
    QString Text = curEdit->toPlainText();
    QStringList list = Text.split('\n');
    foreach ( QString line, list ) {
        line = line.replace( QRegExp( fromText->text() ), toText->text() );
        newText = newText+line+"\n";
    }
    curEdit->setPlainText( newText );
}

void TextReplace::closeEvent( QCloseEvent *event )
{
    Q_UNUSED( event )

    LOGDBG("%s","close event");
    // we can make sure that pattern match nothing by the following way.
    QString pattern = textEdit->toPlainText()+"hahhahaha";
    highlighter = QSharedPointer<SimLighter>( new SimLighter( textEdit->document(), pattern ) );
}

void TextReplace::setTabWidget( QTabWidget *tabWidget )
{
    this->tabWidget = tabWidget;
}

QString TextReplace::normalTitle( QString text )
{
    //    background:#DDDD00;  // the back color is yellow
    QString str = "<font style=\"color:#000000\" size=\"4\">";
    str = str+text;
    str = str+"</font>";
    return str;
}

void TextReplace::setPattern()
{
    pattern = fromText->text();
}
