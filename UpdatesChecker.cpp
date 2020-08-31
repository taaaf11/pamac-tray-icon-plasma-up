#include "UpdatesChecker.h"
#include <glib.h>
namespace LibQPamac {
UpdatesChecker::UpdatesChecker(QObject *parent) : QObject(parent),
    m_handle(pamac_updates_checker_new())
{
    _init();
}

void UpdatesChecker::checkUpdates()
{
    return pamac_updates_checker_check_updates(m_handle);
}

void UpdatesChecker::_init()
{
    g_signal_connect(m_handle, "updates_available", reinterpret_cast<GCallback>(+[](PamacUpdatesChecker* checker, int updates_nb, void* selfPtr){
                         auto self = reinterpret_cast<UpdatesChecker*>(selfPtr);

                         Q_EMIT self->updatesAvailable(updates_nb);
                     }) ,this);
}

} // namespace LibQPamac
