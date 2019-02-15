#include "gui.h"
#include <QApplication>
#include <QWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Gui w;

    w.resize(700, 400); //720 x 480
    w.setWindowTitle("FootCam v0.3.1");
    w.show();

    return a.exec();
}
