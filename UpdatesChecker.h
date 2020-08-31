#pragma once
#include <QObject>
#include <pamac.h>
namespace LibQPamac {
#define LIBQPAMAC_PROPERTY_GET(type, name, method) \
    Q_PROPERTY(type name READ name CONSTANT)\
    inline type name() const \
{\
    return type(method);\
}

class UpdatesChecker : public QObject
{
    Q_OBJECT

public:
    LIBQPAMAC_PROPERTY_GET(bool, noUpdateHideIcon, pamac_updates_checker_get_no_update_hide_icon(m_handle))
    LIBQPAMAC_PROPERTY_GET(uint16_t, updatesNb, pamac_updates_checker_get_updates_nb(m_handle))
    LIBQPAMAC_PROPERTY_GET(uint32_t, refreshPeriod, pamac_updates_checker_get_refresh_period(m_handle))
    explicit UpdatesChecker(QObject *parent = nullptr);
    void checkUpdates();

Q_SIGNALS:
    int updatesAvailable(uint16_t updatesNb);

private:
    void _init();
    PamacUpdatesChecker* m_handle;

};
} // namespace LibQPamac
