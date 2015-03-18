#include "widgetgl.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    bool fullscreen = false;
    QApplication a(argc, argv);

    switch(QMessageBox::information( 0,
                                    QObject::tr("Start Fullscreen?"),
                                    QObject::tr("Would you like to run in fullscreen mode?"),
                                       QMessageBox::Yes,
                                       QMessageBox::No | QMessageBox::Default) )
    {
    case QMessageBox::Yes:
        fullscreen = true;
        break;
    case QMessageBox::No:
        fullscreen = false;
        break;
    }

    WidgetGL w(0, fullscreen);
    a.setActiveWindow(&w);
    w.show();
    
    return a.exec();
}
