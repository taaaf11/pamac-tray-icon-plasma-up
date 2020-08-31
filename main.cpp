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
    notify_init("Package Manager");

    QApplication a(argc, argv);
    QApplication::setQuitOnLastWindowClosed(false);

    PamacTray tray;
    tray.show();

    return QApplication::exec();
}
