#include <QApplication>
#include <libnotify/notify.h>
#include <pamac_tray.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    notify_init("Package Manager");

    PamacTray tray;
    tray.show();

    return a.exec();
}
