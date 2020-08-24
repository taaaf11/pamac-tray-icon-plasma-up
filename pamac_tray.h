#pragma once
#include <QSystemTrayIcon>
#include <QMenu>
#include <QTimer>
#include <pamac.h>
#include <glib.h>
#include <QTimerEvent>
#include <libnotify/notification.h>

class PamacTray : public QSystemTrayIcon
{
    Q_OBJECT
public:
    PamacTray(QObject* parent = nullptr):QSystemTrayIcon(QIcon::fromTheme("pamac-tray-no-update"),parent),
        m_updatesChecker(pamac_updates_checker_new())
    {


        setToolTip("Pamac tray");

        this->setContextMenu(create_menu());
        connect(this, &QSystemTrayIcon::activated, this,
                [=](){
            if(pamac_updates_checker_get_updates_nb(m_updatesChecker)>0){
                execute_updater();
            } else{
                execute_manager();
            }
        });

        g_signal_connect(m_updatesChecker, "updates_available", reinterpret_cast<GCallback>(+[](PamacUpdatesChecker* checker, int updates_nb, void* selfPtr){
                             auto self = reinterpret_cast<PamacTray*>(selfPtr);

                             if(updates_nb == 0){
                                 self->setIcon(QIcon::fromTheme("pamac-tray-no-update"));
                                 self->setToolTip(tr("Your system is up-to-date"));
                                 self->setVisible(!pamac_updates_checker_get_no_update_hide_icon(checker));
                                 self->close_notification();
                             } else{
                                 self->show_or_update_notification(updates_nb);
                             }

                         }), this);

        QTimer::singleShot(30 * 1000, this, [=](){
            pamac_updates_checker_check_updates(m_updatesChecker);
        });
        updateCheckerTimerId = startTimer(3600*1000);

    }

    void timerEvent(QTimerEvent* e) override {
        if (e->timerId() == updateCheckerTimerId){
            pamac_updates_checker_check_updates(m_updatesChecker);
        }
    }
private Q_SLOTS:
    void execute_updater();
    void execute_manager();
    void show_notification(QString info);
    void show_or_update_notification(uint updates_nb);
    void update_notification(QString info);
    void close_notification();
private:
    int updateCheckerTimerId;
    PamacUpdatesChecker* m_updatesChecker = nullptr;
    NotifyNotification* m_notification = nullptr;
    QMenu* create_menu();



};
