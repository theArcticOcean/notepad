#include "./Inc/notepad.h"
#include <QIcon>
#include <QDebug>
#include <QtCore>
#include <QFileDialog>
#include <QMessageBox>
#include <QFontDialog>
#include "./Inc/handler.h"

notePad::notePad(QWidget *parent) : appPath(QCoreApplication::applicationDirPath()),
    QMainWindow(parent)
{
    finder = 0;
    replacer = 0;

    char_map = new charMap("./folder/.map");
    menubar = new QMenuBar();
    timer = new QTimer();
    timer->setInterval(7000);
    timer->start();

    fileMenuInit();
    editMenuInit();
    findMenuInit();
    cryptMenuInit();

    addMenus();

    this->setMenuBar(menubar);
    tabwidget = new QTabWidget();
    tabwidget->setTabsClosable(true);
    if(tabwidget->count()<=0){
        QTextEdit *text = new QTextEdit();
        text->setPlainText("");

        QIcon *icon = new QIcon(":/images/new.png");
        fileName = "new 1";
        tabwidget->addTab(text,*icon,fileName);
    }
    path = appPath+"/backup/"+tabwidget->tabText(tabwidget->currentIndex());

    recorder = new xmlRecorder(tabwidget);
    recorder->readXML();

    this->setCentralWidget(tabwidget);
    QIcon icon;
    icon.addFile(":/images/note.ico");
    this->setWindowIcon(icon);
    this->setGeometry(100,100,800,600);
    newCount = 0;

    connect(tabwidget,SIGNAL(tabCloseRequested(int)),this,SLOT(deleteTab(int)));
    connect(New,SIGNAL(triggered()),this,SLOT(actionNew_triggered()));
    connect(Open,SIGNAL(triggered()),this,SLOT(actionOpen_triggered()));
    connect(Save,SIGNAL(triggered()),this,SLOT(actionSave_triggered()));
    connect(Save_as,SIGNAL(triggered()),this,SLOT(actionSave_as_triggered()));
    connect(Copy,SIGNAL(triggered()),this,SLOT(actionCopy_triggered()));
    connect(Paste,SIGNAL(triggered()),this,SLOT(actionPaste_triggered()));
    connect(Cut,SIGNAL(triggered()),this,SLOT(actionCut_triggered()));
    connect(Undo,SIGNAL(triggered()),this,SLOT(actionUndo_triggered()));
    connect(Redo,SIGNAL(triggered()),this,SLOT(actionRedo_triggered()));
    connect(initFont,SIGNAL(triggered()),this,SLOT(actionInitFont()));
    connect(timer,SIGNAL(timeout()),this,SLOT(backup()));
    connect(find,SIGNAL(triggered()),this,SLOT(actionFind_triggered()));
    connect(replace,SIGNAL(triggered()),this,SLOT(actionReplace_triggered()));
    connect(encrypt,SIGNAL(triggered()),this,SLOT(actionEncrypt()));
    connect(decrypt,SIGNAL(triggered()),this,SLOT(actionDecrypt()));
}

void notePad::fileMenuInit()
{
    fileMenu = new QMenu("file");

    New = new QAction("new",this);
    New->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));    // 0x04000000 + 0x4e
    Open = new QAction("open",this);
    Open->setShortcut(QKeySequence(tr("Ctrl+O")));
    Save = new QAction("save",this);
    Save->setShortcut(tr("Ctrl+S"));
    Save_as = new QAction("save as",this);
    Save_as->setShortcut(tr("Ctrl+Shift+S"));

    fileMenu->addAction(New);
    fileMenu->addAction(Open);
    fileMenu->addAction(Save);
    fileMenu->addAction(Save_as);
}

void notePad::editMenuInit()
{
    editMenu = new QMenu("edit");

    Copy = new QAction("copy",this);
    Copy->setShortcut(QKeySequence(tr("Ctrl+C")));
    Paste = new QAction("paste",this);
    Paste->setShortcut(QKeySequence(tr("Ctrl+V")));
    Cut = new QAction("cut",this);
    Cut->setShortcut(QKeySequence(tr("Ctrl+X")));
    Undo = new QAction("undo",this);
    Undo->setShortcut(QKeySequence(tr("Ctrl+Z")));
    Redo = new QAction("redo",this);
    Redo->setShortcut(QKeySequence(tr("Ctrl+Y")));
    initFont = new QAction("initFont",this);
    initFont->setShortcut(QKeySequence(tr("Ctrl+I")));
    initFont->setToolTip(QObject::tr("set all text with default font:\n"
                                     "font-family: Sans, "
                                     "pointSize: 10, "
                                     "weight: 50, "
                                     "italic: false"));

    editMenu->addAction(Copy);
    editMenu->addAction(Paste);
    editMenu->addAction(Cut);
    editMenu->addAction(Undo);
    editMenu->addAction(Redo);
    editMenu->addAction(initFont);
    editMenu->setToolTipsVisible(true);
}

void notePad::findMenuInit()
{
    findMenu = new QMenu("find");

    find = new QAction("find",this);
    find->setShortcut(QKeySequence(tr("Ctrl+F")));
    replace = new QAction("replace",this);
    replace->setShortcut(QKeySequence(tr("Ctrl+R")));

    findMenu->addAction(find);
    findMenu->addAction(replace);
}

void notePad::cryptMenuInit()
{
    cryptMenu = new QMenu("en/decrypt");

    encrypt = new QAction("encrypt",this);
    encrypt->setShortcut(QKeySequence(tr("Ctrl+E")));
    decrypt = new QAction("decrypt",this);
    decrypt->setShortcut(QKeySequence(tr("Ctrl+D")));

    cryptMenu->addAction(encrypt);
    cryptMenu->addAction(decrypt);
}

void notePad::addMenus()
{
    menubar->addMenu(fileMenu);
    menubar->addMenu(editMenu);
    menubar->addMenu(findMenu);
    menubar->addMenu(cryptMenu);
}

void notePad::actionEncrypt()
{
    int i;
    int key;
    int value;
    int bytes_number;
    char *bytes;
    QWidget *w;
    QTextEdit *textEdit;

    timer->stop();
    char_map->create_map();
    w = tabwidget->currentWidget();
    textEdit = static_cast<QTextEdit *>(w);
    fileName = tabwidget->tabText(tabwidget->currentIndex());
    path = appPath+"/backup/"+fileName;

    bytes = bytesRead(path.toLatin1(),&bytes_number);
    QMap<int,int> text_map = char_map->getMap();
    for(i=0;i<bytes_number;i++){
        key = bytes[i]+128;
        value = text_map[key];
        bytes[i] = value-128;
    }
    bytesWrite(path.toLatin1(),bytes,bytes_number);
    textEdit->setPlainText(bytes);
}
/*
QString::toLatin1() --> QByteArray
QByteArray --> QString
*/
void notePad::actionDecrypt()
{
    int bytes_number;
    char *bytes;
    int i;
    int key;
    int value;
    QWidget *w ;
    QTextEdit *textEdit;
    QString text;

    char_map->read_map();
    w = tabwidget->currentWidget();
    textEdit = static_cast<QTextEdit *>(w);
    fileName = tabwidget->tabText(tabwidget->currentIndex());
    path = appPath+"/backup/"+fileName;
    bytes = bytesRead(path.toLatin1(),&bytes_number);

    QMap<int,int> text_map = char_map->getMap();
    for(i=0;i<bytes_number;i++){
        value = bytes[i]+128;
        key = text_map.key(value); //charMap.key(value);
        bytes[i] = key-128;
    }
    bytesWrite(path.toLatin1(),bytes,bytes_number);
    textEdit->setPlainText(bytes);
    timer->start();
}

void notePad::deleteTab(int index)
{
    tabwidget->removeTab(index);
}

void notePad::actionNew_triggered()
{
    fileName = "new";
    newCount = tabwidget->count();
    newCount++;
    fileName = fileName + " " + QString::number(newCount);
    path = appPath+"/backup/"+fileName;
    QTextEdit *text = new QTextEdit();
    text->setPlainText("");
    QIcon *icon = new QIcon(":/images/new.png");
    tabwidget->addTab(text,*icon,fileName);
    tabwidget->setCurrentWidget(text);
    QString label = QString("new &%1").arg(newCount);
    tabwidget->setTabText(newCount-1,label);
    //qDebug()<<tabwidget->currentIndex();
}

void notePad::actionPaste_triggered()
{
    //static_cast<QTextEdit *> is same as (QTextEdit *)
    QTextEdit *textEdit = (QTextEdit *)(tabwidget->currentWidget());
    textEdit->paste();
}

void notePad::actionCopy_triggered()
{
    QTextEdit *textEdit = (QTextEdit *)(tabwidget->currentWidget());
    textEdit->copy();
}

void notePad::actionCut_triggered()
{
    QTextEdit *textEdit = (QTextEdit *)(tabwidget->currentWidget());
    textEdit->cut();
}

void notePad::actionRedo_triggered()
{
    QTextEdit *textEdit = (QTextEdit *)(tabwidget->currentWidget());
    textEdit->redo();
}

void notePad::actionUndo_triggered()
{
    QTextEdit *textEdit = (QTextEdit *)(tabwidget->currentWidget());
    textEdit->undo();
}

void notePad::actionOpen_triggered()
{
    path = QFileDialog::getOpenFileName(this,"open file");
    fileName = baseName(path);
    QFile gotFile(path);
    if(gotFile.open(QFile::ReadWrite| QFile::Text)){
        QTextStream in(&gotFile);
        QString content = in.readAll();
        gotFile.close();
        actionNew_triggered();
        qint8 tabCount = tabwidget->count();
        QTextEdit *textEdit = (QTextEdit *)tabwidget->widget(tabCount-1);
        textEdit->setPlainText(content);
        tabwidget->setCurrentIndex(tabCount-1);
    }
}

void notePad::actionSave_triggered()
{
    QFile file(path);
    if(file.open(QFile::ReadWrite | QFile::Text | QIODevice::Truncate)){
        QTextStream out(&file);
        QTextEdit *textEdit = (QTextEdit *)(tabwidget->currentWidget());
        out<<textEdit->toPlainText();
        file.flush();
        file.close();
    }
    else QMessageBox::warning(this,"warning","file save failed when try to writing open\n"+path);
}

void notePad::actionSave_as_triggered()
{
    path = QFileDialog::getSaveFileName(this,"save file");
    if(!path.isEmpty()){
        fileName = baseName(path);
        int index = tabwidget->currentIndex();
        tabwidget->setTabText(index,fileName);
        actionSave_triggered();
    }
    else QMessageBox::warning(this,"warning","file name is empty.");
}

void notePad::actionFind_triggered()
{
    QTextEdit *textEdit = (QTextEdit *)tabwidget->currentWidget();
    //qDebug()<<textEdit->document();
    finder = new textFind();
    finder->setTextEdit(textEdit);
    finder->show();
    //highlighter.highlightBlock(textEdit->toPlainText());
}

void notePad::actionReplace_triggered()
{
    QTextEdit *textEdit = (QTextEdit *)tabwidget->currentWidget();
    replacer = new textReplace();
    replacer->setTabWidget(tabwidget);
    replacer->setTextEdit(textEdit);
    // forbid maximize when click father widget.
    replacer->setWindowFlags(replacer->windowFlags() & ~Qt::WindowMaximizeButtonHint);
    // forbid minimize when click father widget.
    // replacer->setWindowFlags(replacer->windowFlags() & ~Qt::WindowMinimized);
    replacer->show();
}

void notePad::actionInitFont()
{
    QWidget *w = tabwidget->currentWidget();
    QTextEdit *textEdit = static_cast<QTextEdit *>(w);
    QString text = textEdit->toPlainText();
    textEdit->setText(text);
    QFont f;
    f.setPixelSize(-1);
    f.setFamily("Sans");
    f.setPointSize(10);
    f.setWeight(50);
    textEdit->selectAll();
    textEdit->setCurrentFont(f);
    textEdit->setTextColor(Qt::black);
}

//void notePad::actionPartFont()
//{
//    bool ok;
//    QFont font = QFontDialog::getFont(
//                    &ok, QFont("Sans",10,50,false), this);
//    if (ok) {
//        QWidget *w = tabwidget->currentWidget();
//        QTextEdit *textEdit = static_cast<QTextEdit *>(w);
//        textEdit->setCurrentFont(font);
//    }
//}

void notePad::backup()
{
    fileName = tabwidget->tabText(tabwidget->currentIndex());
    path = appPath+"/backup/"+fileName;
    qDebug()<<"backup works: "<<path;
    if(tabwidget->count()>0) actionSave_triggered();
}

void notePad::closeEvent(QCloseEvent *event)
{
    if(finder){
        finder->close();
    }
    if(replacer){
        replacer->close();
    }
    recorder->writeXML();
    QWidget::closeEvent(event);
}
