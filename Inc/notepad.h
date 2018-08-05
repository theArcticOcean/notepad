#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QTabWidget>
#include <QTextEdit>
#include <QTimer>
#include <QIcon>
#include <QTranslator>
#include <QMutex>
#include <map>
#include <QSharedPointer>

#include "textfind.h"
#include "textreplace.h"
#include "reglighter.h"
#include "xmlrecorder.h"
#include "charmap.h"

using namespace std;

class NotePad : public QMainWindow
{
    Q_OBJECT

public:
    explicit NotePad(QWidget *parent = 0);
    void fileMenuInit();
    void editMenuInit();
    void findMenuInit();
    void infoMenuInit();
    void addMenus();

    static QMutex mutexForRW;
    static unsigned int encryptBytes;

signals:    
    void changeFileName(QString str);

public slots:
    void actionEncrypt();
    void actionHelp();
    void actionDecrypt();
    void actionEncryptIntrodut();
    void deleteTabAndFile(int index);
    void actionNew_triggered();
    void actionPaste_triggered();
    void actionCopy_triggered();
    void actionCut_triggered();
    void actionRedo_triggered();
    void actionUndo_triggered();
    void actionOpen_triggered();
    void actionSave_triggered();
    void actionSave_as_triggered();
    void actionFind_triggered();
    void actionReplace_triggered();
    void actionInitFont();
    void actionFontIncreaseSize();
    void actionFontReduceSize();
    //void actionPartFont();

    void backup();
    void closeEvent(QCloseEvent *event);

private:
    QSharedPointer<QMenuBar> menubar;
    QSharedPointer<QMenu> fileMenu, editMenu, findMenu, infoMenu;

    QSharedPointer<QAction> New, Open, Save, Save_as;
    QSharedPointer<QAction> Copy, Paste, Cut, Undo, Redo;
    QSharedPointer<QAction> find, replace;
    QSharedPointer<QAction> initFont, increaseFontSize, reduceFontSize;
    QSharedPointer<QAction> introduct, help;
    QSharedPointer<QTabWidget> tabwidget;
    QSharedPointer<CharMap> char_map;
    //QAction *encrypt, *decrypt

    QSharedPointer<QTimer> timer;
    QSharedPointer<TextFind> finder;
    QSharedPointer<TextReplace> replacer;
    QSharedPointer<XmlRecorder> recorder;

    qint8 newCount;
    const QString appPath;
    QString path;
    QString fileName;
};

#endif // NOTEPAD_H
