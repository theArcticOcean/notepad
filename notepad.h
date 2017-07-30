#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QTabWidget>
#include <QTextEdit>
#include <QTimer>
#include "textfind.h"
#include "textreplace.h"
#include "regLighter.h"
#include "xmlrecorder.h"

class notePad : public QMainWindow
{
    Q_OBJECT

    QMenuBar *menubar;
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *findMenu;
    QAction *New, *Open, *Save, *Save_as;
    QAction *Copy,*Paste,*Cut,*Undo,*Redo;
    QAction *find, *replace;
    QAction *initFont;// *partFont;
    QTabWidget *tabwidget;

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

signals:    
    void changeFileName(QString str);
public slots:
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
