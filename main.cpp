#include <QApplication>
#include <QTranslator>
#include <pamac_tray.h>
#include <run_guard.h>
#include <libnotify/notify.h>
int main(int argc, char *argv[])
{
    RunGuard guard("org.manjaro.pamac.tray-plasma");
    if(!guard.tryToRun()){
        return 0;
    }

    QApplication a(argc, argv);
    QApplication::setQuitOnLastWindowClosed(false);

    QTranslator translator;
    if(translator.load(QLocale(),"pamac_tray", "_", ":/i18n", ".qm")){
        QApplication::installTranslator(&translator);
    }

    QApplication::setApplicationName(QApplication::tr("Package Manager"));
    notify_init(QApplication::tr("Package Manager").toUtf8());

    PamacTray tray;

    return QApplication::exec();
}
