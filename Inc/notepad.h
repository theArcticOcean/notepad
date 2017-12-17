#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QTabWidget>
#include <QTextEdit>
#include <QTimer>
#include "./Inc/textfind.h"
#include "./Inc/textreplace.h"
#include "./Inc/regLighter.h"
#include "./Inc/xmlrecorder.h"
#include "./Inc/charmap.h"
#include <QTranslator>

class notePad : public QMainWindow
{
    Q_OBJECT

    QMenuBar *menubar;
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *findMenu;
    QMenu *cryptMenu;

    QAction *New, *Open, *Save, *Save_as;
    QAction *Copy,*Paste,*Cut,*Undo,*Redo;
    QAction *find, *replace;
    QAction *initFont;                  // *partFont;
    QAction *encrypt;
    QAction *decrypt;
    QTabWidget *tabwidget;
    charMap *char_map;

    QTimer *timer;
    qint8 newCount;
    const QString appPath;
    QString path;
    QString fileName;
    textFind *finder;
    textReplace *replacer;
    regLighter *highlighter;
    xmlRecorder *recorder;

public:
    explicit notePad(QWidget *parent = 0);
    void fileMenuInit();
    void editMenuInit();
    void findMenuInit();
    void cryptMenuInit();
    void addMenus();

signals:    
    void changeFileName(QString str);

public slots:
    void actionEncrypt();

    void actionDecrypt();

    void deleteTab(int index);

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

    //void actionPartFont();

    void backup();

    void closeEvent(QCloseEvent *event);
};

#endif // NOTEPAD_H