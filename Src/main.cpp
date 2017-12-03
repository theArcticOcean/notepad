#include <QApplication>
#include "./Inc/notepad.h"

int main(int argc,char **argv){
    QApplication app(argc,argv);
    notePad pad;
    pad.show();
    return app.exec();
}
