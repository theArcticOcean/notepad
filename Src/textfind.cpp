#include <QTextCharFormat>
#include <QColor>
#include <QMessageBox>

#include "textfind.h"
#include "reglighter.h"
#include "simlighter.h"
#include "log.h"
#include "commondata.h"

TextFind::TextFind( QWidget *parent )
    : QWidget( parent )
    , highlighter( NULL )
    , lighterType( NoType )
{
    label = QSharedPointer<QLabel>( new QLabel("find :  ") );
    line = QSharedPointer<QLineEdit>( new QLineEdit() );
    findButton = QSharedPointer<QPushButton>( new QPushButton("find") );
    hLayout1 = QSharedPointer<QHBoxLayout>( new QHBoxLayout() );
    hLayout1->addWidget( label.data());
    hLayout1->addWidget( line.data());
    hLayout1->addWidget( findButton.data());

    vLayout = QSharedPointer<QVBoxLayout>( new QVBoxLayout() );
    vLayout->addLayout( hLayout1.data());

    group1 = QSharedPointer<QGroupBox>( new QGroupBox("find pattern: ") );
    simpleText = QSharedPointer<QRadioButton>( new QRadioButton( "simple text", group1.data() ) );
    regExp = QSharedPointer<QRadioButton>( new QRadioButton( "regular expression", group1.data() ) );
    simpleText->setGeometry( QRect(0,35,92,21));
    regExp->setGeometry( QRect(0,65,92,21));

    hLayout2 = QSharedPointer<QHBoxLayout>( new QHBoxLayout() );
    hLayout2->addWidget( group1.data() );
    vLayout->addLayout( hLayout2.data());
    QIcon icon;
    icon.addFile(": /images/find.ico");
    this->setWindowIcon( icon );
    this->resize(400,180);
    this->setLayout( vLayout.data());
    this->setWindowTitle("find window");
    this->setWindowIcon( QIcon( ICON_PATH ));

    connect( findButton.data(), SIGNAL( clicked()), this, SLOT( setPattern()));
}

TextFind::~TextFind()
{
    deleteHighLighterPtr();
    LOGDBG( "~TextFind() finished." );
}

QString TextFind::getPattern()
{
    return pattern;
}

void TextFind::setTextEdit( QTextEdit *textEdit )
{
    this->textEdit = textEdit;
}

void TextFind::search()
{
    if( pattern.isEmpty()) {
        QMessageBox::warning( this,"find context","please enter your context to find.");
        return ;
    }

    if( regExp->isChecked()){
        deleteHighLighterPtr();
        highlighter = new RegLighter( textEdit->document(), pattern );
        lighterType = regLighterType;
    }
    else if( simpleText->isChecked())
    {
        deleteHighLighterPtr();
        highlighter = new SimLighter( textEdit->document(), pattern );
        lighterType = simLighterType;
    }
    else
    {
        QMessageBox::information( this,"find pattern","please choose a find pattern.");
    }
}

void TextFind::closeEvent( QCloseEvent *event )
{
    LOGDBG("%s","close event");
    // we can make sure that pattern match nothing by the following way.
    QString pattern = textEdit->toPlainText()+"hahhahaha";
    deleteHighLighterPtr();
    QWidget::closeEvent( event );
}

void TextFind::deleteHighLighterPtr()
{
    if( NULL != highlighter )
    {
        if( simLighterType == lighterType )
        {
            SimLighter *tmp = static_cast<SimLighter*>( highlighter );
            delete tmp;
            tmp = NULL;

        }
        else if( regLighterType == lighterType )
        {
            RegLighter *tmp = static_cast<RegLighter*>( highlighter );
            delete tmp;
            tmp = NULL;
        }

        highlighter = NULL;
        lighterType = NoType;
    }
}

void TextFind::setPattern()
{
    LOGDBG("you click find button");
    pattern = line->text();
    search();
}
