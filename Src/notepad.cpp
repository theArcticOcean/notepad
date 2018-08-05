#include <QIcon>
#include <QDebug>
#include <QtCore>
#include <QFileDialog>
#include <QMessageBox>
#include <QFontDialog>
#include <errno.h>
#include <unistd.h>
#include <string>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <QtGlobal>
#include <QDesktopServices>
#include <QUrl>

#include "notepad.h"
#include "log.h"
#include "commondata.h"
#include "handler.h"

unsigned int NotePad::encryptBytes = 0;

NotePad::NotePad( QWidget *parent )
    : QMainWindow( parent )
    , appPath( QCoreApplication::applicationDirPath() )
{
    LOGDBG( "program start run!" );
    if( access( FOLDER_PATH, F_OK ) ){
#ifdef Q_OS_WIN
        if( 0 != mkdir( FOLDER_PATH ) ) {
#else
        if( 0 != mkdir( FOLDER_PATH, 0777 ) ) {
#endif
            LOGDBG( "%s create failed: %s", FOLDER_PATH, strerror( errno ) );
        }
    }
    if( access( BACKUP_PATH, F_OK ) ){
#ifdef Q_OS_WIN
        if( 0 != mkdir( BACKUP_PATH ) ) {
#else
        if( 0 != mkdir( BACKUP_PATH, 0777 ) ) {
#endif
            LOGDBG( "%s create failed: %s", BACKUP_PATH, strerror( errno ) );
        }
    }

    char_map = QSharedPointer<CharMap>( new CharMap( RELATIVE_MAP_FILE_PATH ) );
    menubar = QSharedPointer<QMenuBar>( new QMenuBar() );
    timer = QSharedPointer<QTimer>( new QTimer() );
    timer->setInterval(7000);
    timer->start();

    fileMenuInit();
    editMenuInit();
    findMenuInit();
    infoMenuInit();

    addMenus();

    this->setMenuBar( menubar.data() );
    tabwidget = QSharedPointer<QTabWidget>( new QTabWidget() );
    tabwidget->setTabsClosable( true );

    recorder = QSharedPointer<XmlRecorder>( new XmlRecorder( tabwidget.data() ) );
    recorder->readXML();

    if(0 == tabwidget->count() ){
        QTextEdit *text = new QTextEdit();
        text->setTabChangesFocus( true );
        text->setPlainText( "" );

        QIcon icon( ":/images/new.png" );
        fileName = "new 1";
        tabwidget->addTab( text, icon, fileName );
    }
    path = appPath+"/backup/"+tabwidget->tabText( tabwidget->currentIndex() );
    QString qDir = appPath+"/backup";
    std::string sDir = qDir.toStdString();
    if(0 != access( ( char *)sDir.c_str(), F_OK ) ){

#ifdef Q_OS_WIN
        if(0 != mkdir( ( char *)sDir.c_str() ) ){
#else
        if(-1 == mkdir( ( char *)sDir.c_str(),0777) ){
#endif
            LOGDBG( "folder %s create failed, error: %s", sDir.c_str(), strerror( errno ) );
        }
    }

    this->setCentralWidget( tabwidget.data() );
    this->setGeometry( 100, 100, 800, 600 );

    newCount = 0;
    this->setWindowIcon( QIcon( ICON_PATH ) );

    connect( tabwidget.data(), SIGNAL( tabCloseRequested( int ) ), this, SLOT( deleteTabAndFile( int ) ) );
    connect( New.data(), SIGNAL( triggered() ), this, SLOT( actionNew_triggered() ) );
    connect( Open.data(), SIGNAL( triggered() ), this, SLOT( actionOpen_triggered() ) );
    connect( Save.data(), SIGNAL( triggered() ), this, SLOT( actionSave_triggered() ) );
    connect( Save_as.data(), SIGNAL( triggered() ), this, SLOT( actionSave_as_triggered() ) );
    connect( Copy.data(), SIGNAL( triggered() ), this, SLOT( actionCopy_triggered() ) );
    connect( Paste.data(), SIGNAL( triggered() ), this, SLOT( actionPaste_triggered() ) );
    connect( Cut.data(), SIGNAL( triggered() ), this, SLOT( actionCut_triggered() ) );
    connect( Undo.data(), SIGNAL( triggered() ), this, SLOT( actionUndo_triggered() ) );
    connect( Redo.data(), SIGNAL( triggered() ), this, SLOT( actionRedo_triggered() ) );
    connect( initFont.data(), SIGNAL( triggered() ), this, SLOT( actionInitFont() ) );
    connect( increaseFontSize.data(), SIGNAL( triggered( bool ) ), this, SLOT( actionFontIncreaseSize() ) );
    connect( reduceFontSize.data(), SIGNAL( triggered( bool ) ), this, SLOT( actionFontReduceSize() ) );
    connect( timer.data(), SIGNAL( timeout() ), this, SLOT( backup() ) );
    connect( find.data(), SIGNAL( triggered() ), this, SLOT( actionFind_triggered() ) );
    connect( replace.data(), SIGNAL( triggered() ), this, SLOT( actionReplace_triggered() ) );
    connect( introduct.data(), SIGNAL( triggered( bool ) ), this, SLOT( actionEncryptIntrodut() ) );
    connect( help.data(), SIGNAL( triggered( bool ) ), this, SLOT( actionHelp() ) );
    //    connect( encrypt, SIGNAL( triggered() ), this, SLOT( actionEncrypt() ) );
    //    connect( decrypt, SIGNAL( triggered() ), this, SLOT( actionDecrypt() ) );
}

void NotePad::fileMenuInit()
{
    fileMenu = QSharedPointer<QMenu>( new QMenu( "file" ) );

    New = QSharedPointer<QAction>( new QAction( "new", this ) );
    New->setShortcut( QKeySequence( Qt::CTRL + Qt::Key_N ) );    // 0x04000000 + 0x4e
    Open = QSharedPointer<QAction>( new QAction( "open", this ) );
    Open->setShortcut( QKeySequence( tr( "Ctrl+O" ) ) );
    Save = QSharedPointer<QAction>( new QAction( "save", this ) );
    Save->setShortcut( tr( "Ctrl+S" ) );
    Save_as = QSharedPointer<QAction>( new QAction( "save as", this ) );
    Save_as->setShortcut( tr( "Ctrl+Shift+S" ) );

    fileMenu->addAction( New.data() );
    fileMenu->addAction( Open.data() );
    fileMenu->addAction( Save.data() );
    fileMenu->addAction( Save_as.data() );
}

void NotePad::editMenuInit()
{
    editMenu = QSharedPointer<QMenu>( new QMenu( "edit" ) );

    Copy = QSharedPointer<QAction>( new QAction( "copy", this ) );
    Copy->setShortcut( QKeySequence( tr( "Ctrl+C" ) ) );
    Paste = QSharedPointer<QAction>( new QAction( "paste", this ) );
    Paste->setShortcut( QKeySequence( tr( "Ctrl+V" ) ) );
    Cut = QSharedPointer<QAction>( new QAction( "cut", this ) );
    Cut->setShortcut( QKeySequence( tr( "Ctrl+X" ) ) );
    Undo = QSharedPointer<QAction>( new QAction( "undo", this ) );
    Undo->setShortcut( QKeySequence( tr( "Ctrl+Z" ) ) );
    Redo = QSharedPointer<QAction>( new QAction( "redo", this ) );
    Redo->setShortcut( QKeySequence( tr( "Ctrl+Y" ) ) );
    initFont = QSharedPointer<QAction>( new QAction( "initFont", this ) );
    initFont->setShortcut( QKeySequence( tr( "Ctrl+I" ) ) );
    initFont->setToolTip( QObject::tr( "set all text with default font:\n"
                                     "font-family: Sans, "
                                     "pointSize: 10, "
                                     "weight: 50, "
                                     "italic: false" ) );
    increaseFontSize = QSharedPointer<QAction>( new QAction( "increase font size", this ) );
    increaseFontSize->setShortcut( QKeySequence( tr( "Ctrl + +" ) ) );
    reduceFontSize = QSharedPointer<QAction>( new QAction( "reduce font size", this ) );
    reduceFontSize->setShortcut( QKeySequence( tr( "Ctrl + -" ) ) );

    editMenu->addAction( Copy.data() );
    editMenu->addAction( Paste.data() );
    editMenu->addAction( Cut.data() );
    editMenu->addAction( Undo.data() );
    editMenu->addAction( Redo.data() );
    editMenu->addAction( initFont.data() );
    editMenu->setToolTipsVisible( true );
    editMenu->addAction( increaseFontSize.data() );
    editMenu->addAction( reduceFontSize.data() );
}

void NotePad::findMenuInit()
{
    findMenu = QSharedPointer<QMenu>( new QMenu( "find" ) );
    find = QSharedPointer<QAction>( new QAction( "find", this ) );
    find->setShortcut( QKeySequence( tr( "Ctrl+F" ) ) );
    replace =  QSharedPointer<QAction>( new QAction( "replace", this ) );
    replace->setShortcut( QKeySequence( tr( "Ctrl+R" ) ) );

    findMenu->addAction( find.data() );
    findMenu->addAction( replace.data() );
}

void NotePad::infoMenuInit()
{
    infoMenu = QSharedPointer<QMenu>( new QMenu( "info" ) );
    introduct = QSharedPointer<QAction>( new QAction( "introduction", this ) );
    introduct->setShortcut( QKeySequence( tr( "Ctrl+Shift+I" ) ) );
    help = QSharedPointer<QAction>( new QAction( "help", this ) );
    help->setShortcut( QKeySequence( tr( "Ctrl+Shift+H" ) ) );

    infoMenu->addAction( introduct.data() );
    infoMenu->addAction( help.data() );

//    infoMenu->addAction( encrypt );
//    infoMenu->addAction( decrypt );
//    encrypt = new QAction( "encrypt", this );
//    encrypt->setShortcut( QKeySequence( tr( "Ctrl+E" ) ) );
//    decrypt = new QAction( "decrypt", this );
//    decrypt->setShortcut( QKeySequence( tr( "Ctrl+Shift+D" ) ) );
}

void NotePad::addMenus()
{
    menubar->addMenu( fileMenu.data() );
    menubar->addMenu( editMenu.data() );
    menubar->addMenu( findMenu.data() );
    menubar->addMenu( infoMenu.data() );
}
void NotePad::actionEncryptIntrodut()
{
    QMessageBox msgBox;
//    msgBox.setText( "You need to know before using it:" );
//    msgBox.setInformativeText( "Encrypt and Decrypt functions are just used to \
//protect your English text which you are writing.\n\
//Don't use it as a encrypt tool." );
    msgBox.setText( "about the product:" );
    msgBox.setInformativeText( "\
myNote is a simple note editor. It support:\n\
1. basic edit functions.\n\
2. find in simple way or regular expression way.\n\
3. replace words in simple way or regular expression way.\n\
4. keep a writing file backup at every 7 seconds.\n\
5. choosing different tab by Alt + tabNumber.\n\
6. increase or reduce font size for writing text.\n\n\
The product has three diffrent versions which are mac, \n\
window and Linux. Its version is 2.0\n\
For more infomation, please visit http://weiy.org \n\
Enjoy it!" );
    msgBox.setDefaultButton( QMessageBox::Ok );
    msgBox.setWindowIcon( QIcon( ICON_PATH ) );
    msgBox.exec();
}

void NotePad::actionHelp()
{
    QDesktopServices::openUrl( QUrl( "http://weiy.org/2017/12/26/mynote-%E5%B8%AE%E5%8A%A9/" ) );
}

void NotePad::actionEncrypt()
{
    int i;
    int key;
    int value;
    FILE *fp;
    int ret;
    char *bytes;
    QWidget *w;
    QTextEdit *textEdit;
    QString text;

    bytes = NULL;
    timer->stop();
    char_map->create_map();
    w = tabwidget->currentWidget();
    textEdit = static_cast<QTextEdit *>( w );
    fileName = tabwidget->tabText( tabwidget->currentIndex() );
    path = appPath+"/backup/"+fileName;
    text = textEdit->toPlainText();
    fp = fopen( path.toStdString().c_str(), "w" ); //O_WRONLY | O_CREAT | O_TRUNC );
    if( NULL == fp ){
        LOGDBG( "%s open failed", path.toStdString().c_str() );
        return ;
    }
    bytes = ( char *)malloc( text.length()+1);
    memcpy( bytes, text.toStdString().c_str(), text.length() );
    bytes[text.length()] = 0;
    QMap<int, int> text_map = char_map->getMap();
    for( i=0;i<text.length();i++){
        key = bytes[i]+128;
        value = text_map[key];
        bytes[i] = value-128;
    }
    textEdit->setText( bytes );
    ret = fwrite( bytes, 1, text.length()+1, fp );
    encryptBytes = text.length()+1;
    LOGDBG( "save : %s", bytes );
    if( NULL != bytes ){
        free( bytes );
        bytes = NULL;
    }
    fclose( fp );
}
/*
QString::toLatin1() --> QByteArray
QByteArray --> QString
*/
void NotePad::actionDecrypt()
{
    char *bytes;
    FILE *fp;
    unsigned int i;
    int ret;
    QWidget *w ;
    QTextEdit *textEdit;
    QString text;

    bytes = NULL;
    if( char_map.data() == NULL || tabwidget.data() == NULL ){
        LOGDBG( "params are illegal" );
        return;
    }
    char_map->read_map();
    w = tabwidget->currentWidget();
    textEdit = static_cast<QTextEdit *>( w );
    fileName = tabwidget->tabText( tabwidget->currentIndex() );
    path = appPath+"/backup/"+fileName;
    fp = fopen( path.toStdString().c_str(),"r" );
    if( NULL == fp ){
        LOGDBG( "fopen failed: %s", strerror( errno ) );
        return ;
    }
    bytes = ( char *)malloc( encryptBytes );
    if( NULL == bytes ){
        LOGDBG( "bytes is NULL" );
        return ;
    }
    ret = fread( bytes,1, encryptBytes, fp );
    if( ret <= 0){
        LOGDBG( "fread ret <= 0, %s", strerror( errno ) );
        free( bytes );
        bytes = NULL;
        return ;
    }
    LOGDBG( "read: %s", bytes );
    QMap<int, int> myMap = char_map->getMap();
    for( i = 0; i < encryptBytes - 1; ++i ){
        int value = bytes[i]+128;
        int key = myMap.key( value );
        bytes[i] = key-128;
    }
    bytes[encryptBytes] = 0;
    textEdit->setText( bytes );

    if( NULL != bytes ){
        free( bytes );
        bytes = NULL;
    }
    fclose( fp );
    actionSave_triggered();
    timer->start();
}

void NotePad::deleteTabAndFile( int index )
{
    QString tabFile = tabwidget->tabText( index );
    tabwidget->removeTab( index );
    std::string fileName = tabFile.toStdString();
    int ret = -9999;
    if( 0 == access( fileName.c_str(), F_OK ) )
    {
        ret = unlink( fileName.c_str() );
    }
    LOGDBG( "rm file %s result: %d", fileName.c_str(), ret );
}

void NotePad::actionNew_triggered()
{
    fileName = "new";
    newCount = tabwidget->count();
    newCount++;
    fileName = fileName + " " + QString::number( newCount );
    path = appPath+"/backup/"+fileName;
    QTextEdit *text = new QTextEdit();
    text->setPlainText( "" );
    QIcon icon( ":/images/new.png" );
    tabwidget->addTab( text, icon, fileName );
    tabwidget->setCurrentWidget( text );
    QString label = QString( "new &%1" ).arg( newCount );
    tabwidget->setTabText( newCount-1, label );
}

void NotePad::actionPaste_triggered()
{
    //static_cast<QTextEdit *> is same as ( QTextEdit *)
    QTextEdit *textEdit = ( QTextEdit *)( tabwidget->currentWidget() );
    textEdit->paste();
}

void NotePad::actionCopy_triggered()
{
    QTextEdit *textEdit = ( QTextEdit *)( tabwidget->currentWidget() );
    textEdit->copy();
}

void NotePad::actionCut_triggered()
{
    QTextEdit *textEdit = ( QTextEdit *)( tabwidget->currentWidget() );
    textEdit->cut();
}

void NotePad::actionRedo_triggered()
{
    QTextEdit *textEdit = ( QTextEdit *)( tabwidget->currentWidget() );
    textEdit->redo();
}

void NotePad::actionUndo_triggered()
{
    QTextEdit *textEdit = ( QTextEdit *)( tabwidget->currentWidget() );
    textEdit->undo();
}

void NotePad::actionOpen_triggered()
{
    path = QFileDialog::getOpenFileName( this,"open file" );
    fileName = baseName( path );
    QFile gotFile( path );
    if( gotFile.open( QFile::ReadWrite| QFile::Text ) ){
        QTextStream in(&gotFile );
        QString content = in.readAll();
        gotFile.close();
        actionNew_triggered();
        qint8 tabCount = tabwidget->count();
        QTextEdit *textEdit = ( QTextEdit *)tabwidget->widget( tabCount-1);
        textEdit->setPlainText( content );
        tabwidget->setCurrentIndex( tabCount-1);
    }
}

void NotePad::actionSave_triggered()
{
    QFile file( path );
    if( file.open( QFile::ReadWrite | QFile::Text | QIODevice::Truncate ) ){
        QTextStream out(&file );
        QTextEdit *textEdit = ( QTextEdit *)( tabwidget->currentWidget() );
        out<<textEdit->toPlainText();
        qDebug()<<textEdit->toPlainText();
        file.flush();
        file.close();
    }
    else {
        QMessageBox::warning( this,"warning","file save failed when try to writing open\n"+path );
    }
}

void NotePad::actionSave_as_triggered()
{
    QFileDialog dialog;
    dialog.setWindowIcon( QIcon( "image/logo.icns" ) );
    path = dialog.getSaveFileName( this,"save file", QCoreApplication::applicationDirPath(),"*.*" );
    if(!path.isEmpty() ){
        fileName = baseName( path );
        int index = tabwidget->currentIndex();
        tabwidget->setTabText( index, fileName );
        actionSave_triggered();
    }
    LOGDBG( "finished." );
}

void NotePad::actionFind_triggered()
{
    QTextEdit *textEdit = ( QTextEdit *)tabwidget->currentWidget();
    finder = QSharedPointer<TextFind>( new TextFind() );
    finder->setTextEdit( textEdit );
    finder->show();
}

void NotePad::actionReplace_triggered()
{
    QTextEdit *textEdit = ( QTextEdit *)tabwidget->currentWidget();
    replacer = QSharedPointer<TextReplace>( new TextReplace() );
    replacer->setTabWidget( tabwidget.data() );
    replacer->setTextEdit( textEdit );
    // forbid maximize when click father widget.
    replacer->setWindowFlags( replacer->windowFlags() & ~Qt::WindowMaximizeButtonHint );
    // forbid minimize when click father widget.
    // replacer->setWindowFlags( replacer->windowFlags() & ~Qt::WindowMinimized );
    replacer->show();
}

void NotePad::actionInitFont()
{
    QWidget *w = tabwidget->currentWidget();
    QTextEdit *textEdit = static_cast<QTextEdit *>( w );

    QString text = textEdit->toPlainText();
    textEdit->setText( "" );

    QFont f;
    f.setPixelSize(-1);
    f.setPointSize(13);
    f.setWeight(50);
    f.setStyle( QFont::StyleNormal );
    f.setFamily( ".SF NS Text" );
    f.setBold( false );

    textEdit->selectAll();
    textEdit->setCurrentFont( f );
    textEdit->setTextColor( Qt::black );
    textEdit->setText( text );
}

void NotePad::actionFontIncreaseSize()
{
    QWidget *w = tabwidget->currentWidget();
    QTextEdit *textEdit = static_cast<QTextEdit *>( w );
    QFont font = textEdit->currentFont();
    font.setPointSize( font.pointSize()+2);
    textEdit->setCurrentFont( font );
    QString text = textEdit->toPlainText();
    textEdit->setText( text );
}

void NotePad::actionFontReduceSize()
{
    QWidget *w = tabwidget->currentWidget();
    QTextEdit *textEdit = static_cast<QTextEdit *>( w );
    QFont font = textEdit->currentFont();
    font.setPointSize( font.pointSize()-2);
    textEdit->setCurrentFont( font );
    QString text = textEdit->toPlainText();
    textEdit->setText( text );
}

void NotePad::backup()
{
    fileName = tabwidget->tabText( tabwidget->currentIndex() );
    path = appPath+"/backup/"+fileName;
    //LOGDBG( "%s%s","backup works: ", path.toStdString().c_str() );
    if( tabwidget->count()>0) actionSave_triggered();
}

void NotePad::closeEvent( QCloseEvent *event )
{
    if( NULL != finder.data() )
    {
        finder->close();
    }

    if( NULL != replacer.data() )
    {
        replacer->close();
    }

    recorder->writeXML();
    QWidget::closeEvent( event );
}
