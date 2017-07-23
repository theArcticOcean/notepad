#include "notepad.h"
#include <QIcon>
#include <QDebug>
#include <QtCore>
#include <QFileDialog>
#include <QMessageBox>
#include <QFontDialog>
#include "handler.h"

notePad::notePad(QWidget *parent) : appPath(QCoreApplication::applicationDirPath()),
    QMainWindow(parent)
{
    finder = 0;
    replacer = 0;

    menubar = new QMenuBar();
    timer = new QTimer();
    timer->setInterval(5000);
    timer->start();
    fileMenu = new QMenu("file");

    New = new QAction("new",this);
    New->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));    // 0x04000000 + 0x4e
    //New->setShortcut(QKeySequence(QLatin1String("Ctrl+N")));
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

    partFont = new QAction("partFont",this);
    partFont->setShortcut(QKeySequence(tr("Ctrl+P")));
    partFont->setToolTip(tr("set partial text with user's font\n"
                            "which is chosen in font dialog."));


    editMenu->addAction(Copy);
    editMenu->addAction(Paste);
    editMenu->addAction(Cut);
    editMenu->addAction(Undo);
    editMenu->addAction(Redo);
    editMenu->addAction(initFont);
    editMenu->addAction(partFont);
    editMenu->setToolTipsVisible(true);

    findMenu = new QMenu("find");
    find = new QAction("find",this);
    find->setShortcut(QKeySequence(tr("Ctrl+F")));
    findMenu->addAction(find);
    replace = new QAction("replace",this);
    replace->setShortcut(QKeySequence(tr("Ctrl+R")));
    findMenu->addAction(replace);

    menubar->addMenu(fileMenu);
    menubar->addMenu(editMenu);
    menubar->addMenu(findMenu);

    this->setMenuBar(menubar);
    tabwidget = new QTabWidget();
    tabwidget->setTabsClosable(true);
    recorder = new xmlRecorder(tabwidget);
    recorder->readXML();
    if(tabwidget->count()<=0){
        QTextEdit *text = new QTextEdit();
        text->setPlainText("");

        QIcon *icon = new QIcon(":/images/new.png");
        fileName = "new 1";
        tabwidget->addTab(text,*icon,fileName);
    }
    path = appPath+"/backup/"+tabwidget->tabText(tabwidget->currentIndex());

    connect(tabwidget,SIGNAL(tabCloseRequested(int)),this,SLOT(deleteTab(int)));
    this->setCentralWidget(tabwidget);
    QIcon icon;
    icon.addFile(":/images/note.ico");
    this->setWindowIcon(icon);
    this->setGeometry(100,100,800,600);
    newCount = 0;

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
    connect(partFont,SIGNAL(triggered()),this,SLOT(actionPartFont()));

    connect(timer,SIGNAL(timeout()),this,SLOT(backup()));
    connect(find,SIGNAL(triggered()),this,SLOT(actionFind_triggered()));
    connect(replace,SIGNAL(triggered()),this,SLOT(actionReplace_triggered()));
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
    QFont f("Sans",10,50,false);
    textEdit->selectAll();
    textEdit->setCurrentFont(f);
    textEdit->setTextColor(Qt::black);
}

void notePad::actionPartFont()
{
    bool ok;
    QFont font = QFontDialog::getFont(
                    &ok, QFont("Sans",10,50,false), this);
    if (ok) {
        QWidget *w = tabwidget->currentWidget();
        QTextEdit *textEdit = static_cast<QTextEdit *>(w);
        textEdit->setCurrentFont(font);
    }
}

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
