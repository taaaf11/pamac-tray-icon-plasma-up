#pragma once
#include <QSystemTrayIcon>
#include <QMenu>
#include <QTimer>
#include <glib.h>
#include <QTimerEvent>
#include <UpdatesChecker.h>
#include <libnotify/notification.h>

class PamacTray : public QSystemTrayIcon
{
    Q_OBJECT
public:
    PamacTray(QObject* parent = nullptr);

    void timerEvent(QTimerEvent* e) override {
        if (e->timerId() == updateCheckerTimerId){
            m_updatesChecker.checkUpdates();
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
    LibQPamac::UpdatesChecker m_updatesChecker;
    NotifyNotification* m_notification = nullptr;
    QMenu* create_menu();



};
