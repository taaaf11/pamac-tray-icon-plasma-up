#include <QApplication>
#include <pamac_tray.h>
#include <run_guard.h>
#include <libnotify/notify.h>
int main(int argc, char *argv[])
{
    RunGuard guard("org.manjaro.pamac.tray-plasma");
    if(!guard.tryToRun()){
        return 0;
    }
    notify_init("pamac-tray-plasma");

    QApplication a(argc, argv);
    QApplication::setQuitOnLastWindowClosed(false);
    QApplication::setApplicationName("pamac-tray-plasma");
    PamacTray tray;


    return QApplication::exec();
}
