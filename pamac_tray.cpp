#include "pamac_tray.h"
#include <QProcess>
#include <QAction>
#include <QApplication>

void PamacTray::execute_updater()
{
    QProcess::startDetached("pamac-manager", {"--updates"});
}

void PamacTray::execute_manager()
{
    QProcess::startDetached("pamac-manager", {{}});

}

void PamacTray::show_notification(QString info)
{
    close_notification();

    m_notification = notify_notification_new(tr("Package Manager").toUtf8(), info.toUtf8(), "system-software-install-symbolic");

    notify_notification_set_timeout(m_notification, NOTIFY_EXPIRES_DEFAULT);

    notify_notification_add_action(m_notification, "default", tr("Details").toUtf8(),+[](NotifyNotification*, char*, void* selfPtr){
        auto self = reinterpret_cast<PamacTray*>(selfPtr);
        self->execute_updater();
    },this,nullptr);

    notify_notification_show(m_notification,nullptr);
}

void PamacTray::show_or_update_notification(uint updates_nb)
{
    QString info = tr("%n available update(s)", "", updates_nb);

    setIcon(QIcon::fromTheme("pamac-tray-update"));
    setToolTip(info);
    setVisible(true);

    update_notification(info);
}

void PamacTray::update_notification(QString info)
{
    if(m_notification!=nullptr){
        if(notify_notification_get_closed_reason(m_notification) == -1)
            notify_notification_update(m_notification, tr("Package Manager").toUtf8(),
                                       info.toUtf8(), "system-software-install-symbolic");

        notify_notification_show(m_notification,nullptr);

    } else{
        show_notification(info);
    }
}

void PamacTray::close_notification()
{
    notify_notification_close(m_notification,nullptr);
    m_notification = nullptr;
}

QMenu* PamacTray::create_menu()
{
    auto menu = new QMenu();

    auto action = new QAction(menu);
    action->setText(tr("Package Manager"));
    connect(action, &QAction::triggered, this, &PamacTray::execute_manager);
    menu->addAction(action);

    action = new QAction(menu);
    action->setText(tr("Quit"));
    connect(action, &QAction::triggered, [](){QApplication::quit();});
    menu->addAction(action);

    return menu;
}
