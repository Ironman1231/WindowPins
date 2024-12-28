#include "WindowPins.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Set the application icon (using resource file)
    a.setWindowIcon(QIcon(":/WindowPins/Icon/Trigger.png"));

    WindowPins w;
    w.setWindowTitle("WindowPins");
    w.show();
    return a.exec();
}
