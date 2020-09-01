#include "pamac_tray.h"
#include <QProcess>
#include <QAction>
#include <QApplication>

PamacTray::PamacTray(QObject *parent):KStatusNotifierItem(parent)
{


    setToolTipTitle(tr("Your system is up-to-date"));
    setIconByName("pamac-tray-no-update");
    setStandardActionsEnabled(false);

    this->setContextMenu(create_menu());
    connect(this, &KStatusNotifierItem::activateRequested, this,
            [=](){
        if(m_updatesChecker.updatesNb()>0){
            execute_updater();
        } else{
            execute_manager();
        }
    });

    connect(&m_updatesChecker, &LibQPamac::UpdatesChecker::updatesAvailable, [this](uint16_t updates_nb){
        if(updates_nb == 0){
            setIconByName("pamac-tray-no-update");
            setToolTipTitle(tr("Your system is up-to-date"));
            setStatus(m_updatesChecker.noUpdateHideIcon()?ItemStatus::Passive:ItemStatus::Active);
            close_notification();
        } else{
            show_or_update_notification(m_updatesChecker.updatesNb());
        }

    });

    QTimer::singleShot(1, this, [=](){
        m_updatesChecker.checkUpdates();
    });
    updateCheckerTimerId = startTimer(3600*1000);

}

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
    m_info = info;
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

    setIconByName("pamac-tray-update");
    setToolTipTitle(info);
    setStatus(ItemStatus::Active);

    update_notification(info);
}

void PamacTray::update_notification(QString info)
{
    if(m_notification!=nullptr){
        if(notify_notification_get_closed_reason(m_notification) == -1 && m_info!=info){
            notify_notification_update(m_notification, tr("Package Manager").toUtf8(),
                                       info.toUtf8(), "system-software-install-symbolic");
            m_info = info;
            notify_notification_show(m_notification,nullptr);
        }

    } else{
        show_notification(info);
    }
}

void PamacTray::close_notification()
{
    if(m_notification!=nullptr){
        notify_notification_close(m_notification,nullptr);
        free(m_notification);
        m_notification = nullptr;
    }
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
    connect(action, &QAction::triggered,this , &QApplication::quit);
    menu->addAction(action);

    return menu;
}
