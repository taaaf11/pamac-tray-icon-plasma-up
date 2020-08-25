#include <QApplication>
#include <libnotify/notify.h>
#include <pamac_tray.h>
#include <run_guard.h>
int main(int argc, char *argv[])
{
    RunGuard guard("org.manjaro.pamac.tray-plasma");
    if(!guard.tryToRun()){
        return 0;
    }

    QApplication a(argc, argv);
    notify_init("Package Manager");

    PamacTray tray;
    tray.show();

    return a.exec();
}
