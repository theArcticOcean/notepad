#include <QApplication>

#include "log.h"
#include "notepad.h"

int main(int argc,char **argv){
    QApplication app(argc,argv);
    LogInit();
    NotePad pad;
    pad.show();
    return app.exec();
}
