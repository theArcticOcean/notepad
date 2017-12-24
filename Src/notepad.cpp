#include "./Inc/notepad.h"
#include "Inc/log.h"
#include "Inc/commondata.h"
#include "./Inc/handler.h"
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

int notePad::encryptBytes = 0;
notePad::notePad(QWidget *parent) : appPath(QCoreApplication::applicationDirPath()),
    QMainWindow(parent)
{
    LOGDBG("program start run!");
    if(access(FOLDER_PATH,F_OK)){
        if(0 != mkdir(FOLDER_PATH,0777)){
            LOGDBG("%s create failed: %s",FOLDER_PATH,strerror(errno));
        }
    }
    if(access(BACKUP_PATH,F_OK)){
        if(0 != mkdir(BACKUP_PATH,0777)){
            LOGDBG("%s create failed: %s",BACKUP_PATH,strerror(errno));
        }
    }

    finder = 0;
    replacer = 0;
    encryptBytes = 0;

    char_map = new charMap(RELATIVE_MAP_FILE_PATH);
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

    recorder = new xmlRecorder(tabwidget);
    recorder->readXML();

    if(0 == tabwidget->count()){
        QTextEdit *text = new QTextEdit();
        text->setPlainText("");

        QIcon *icon = new QIcon(":/images/new.png");
        fileName = "new 1";
        tabwidget->addTab(text,*icon,fileName);
    }
    path = appPath+"/backup/"+tabwidget->tabText(tabwidget->currentIndex());
    QString qDir = appPath+"/backup";
    std::string sDir = qDir.toStdString();
    if(0 != access((char *)sDir.c_str(), F_OK)){
        if(-1 == mkdir((char *)sDir.c_str(),0777)){
            LOGDBG("folder %s create failed, error: %s", sDir.c_str(),strerror(errno));
        }
    }

    this->setCentralWidget(tabwidget);
//    QIcon icon;
//    icon.addFile("logo.icns");
//    this->setWindowIcon(icon);
    this->setGeometry(100,100,800,600);
    newCount = 0;

    connect(tabwidget,SIGNAL(tabCloseRequested(int)),this,SLOT(deleteTabAndFile(int)));
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
    if(0 == encryptBytes)
    {
        int i;
        int key;
        int value;
        int bytes_number;
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
        textEdit = static_cast<QTextEdit *>(w);
        fileName = tabwidget->tabText(tabwidget->currentIndex());
        path = appPath+"/backup/"+fileName;
        text = textEdit->toPlainText();
        fp = fopen(path.toStdString().c_str(), "w"); //O_WRONLY | O_CREAT | O_TRUNC);
        if(NULL == fp){
            LOGDBG("%s open failed",path.toStdString().c_str());
            return ;
        }
        ret = fwrite(text.toStdString().c_str(), 1, text.length(), fp);
        if(ret <= 0){
            LOGDBG("fwrite: %s",strerror(errno));
        }
        encryptBytes = text.length();
        bytes = (char *)malloc(text.length());

        QMap<int,int> text_map = char_map->getMap();
        for(i=0;i<text.length();i++){
            key = bytes[i]+128;
            value = text_map[key];
            bytes[i] = value-128;
        }
        textEdit->setText(bytes);
        if(NULL != bytes){
            free(bytes);
            bytes = NULL;
        }
        fclose(fp);
        LOGDBG("save in %s: %s", path.toStdString().c_str(), text.toStdString().c_str());
    }
}
/*
QString::toLatin1() --> QByteArray
QByteArray --> QString
*/
void notePad::actionDecrypt()
{
    if(encryptBytes > 0)
    {
        int bytes_number;
        char *bytes;
        FILE *fp;
        int i;
        int ret;

        bytes = NULL;
        QWidget *w ;
        QTextEdit *textEdit;
        QString text;

        char_map->read_map();
        w = tabwidget->currentWidget();
        textEdit = static_cast<QTextEdit *>(w);
        fileName = tabwidget->tabText(tabwidget->currentIndex());
        path = appPath+"/backup/"+fileName;
        fp = fopen(path.toStdString().c_str(),"r");
        if(NULL == fp){
            LOGDBG("fopen failed: %s", strerror(errno));
            return ;
        }
        if(encryptBytes <= 0){
            LOGDBG("encryptBytes <= 0");
        }
        bytes = (char *)malloc(encryptBytes);
        ret = fread(bytes,1,encryptBytes,fp);
        if(ret <= 0){
            LOGDBG("fread ret <= 0");
        }
        bytes[encryptBytes] = 0;
        LOGDBG("====> %s",bytes);
        textEdit->setText(bytes);
        if(NULL != bytes){
            free(bytes);
            bytes = NULL;
        }
        fclose(fp);
        actionSave_triggered();
        timer->start();
        encryptBytes = 0;
    }
}

void notePad::deleteTabAndFile(int index)
{
    QString tabFile = tabwidget->tabText(index);
    tabwidget->removeTab(index);
    std::string fileName = tabFile.toStdString();
    int ret;
    if(0 == access(fileName.c_str(),F_OK)){
        ret = unlink(fileName.c_str());
    }
    LOGDBG("rm file %s result: %d", fileName.c_str(),ret);
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
    fileName = tabwidget->tabText(tabwidget->currentIndex());
    path = appPath+"/backup/"+fileName;
    QFile file(path);
    if(file.open(QFile::ReadWrite | QFile::Text | QIODevice::Truncate)){
        QTextStream out(&file);
        QTextEdit *textEdit = (QTextEdit *)(tabwidget->currentWidget());
        out<<textEdit->toPlainText();
        qDebug()<<textEdit->toPlainText();
        file.flush();
        file.close();
    }
    else {
        QMessageBox::warning(this,"warning","file save failed when try to writing open\n"+path);
    }
}

void notePad::actionSave_as_triggered()
{
    QFileDialog dialog;
    dialog.setWindowIcon(QIcon("image/logo.icns"));
    path = dialog.getSaveFileName(this,"save file",QCoreApplication::applicationDirPath(),"*.*");
    if(!path.isEmpty()){
        fileName = baseName(path);
        int index = tabwidget->currentIndex();
        tabwidget->setTabText(index,fileName);
        actionSave_triggered();
    }
    LOGDBG("finished.");
}

void notePad::actionFind_triggered()
{
    QTextEdit *textEdit = (QTextEdit *)tabwidget->currentWidget();
    finder = new textFind();
    finder->setTextEdit(textEdit);
    finder->show();
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

void notePad::backup()
{
    fileName = tabwidget->tabText(tabwidget->currentIndex());
    path = appPath+"/backup/"+fileName;
    //LOGDBG("%s%s","backup works: ",path.toStdString().c_str());
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
