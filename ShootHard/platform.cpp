#include "platform.h"

namespace core
{

    void init(bool _enableMemTrack /*= CORE_MEMORY_TRACKING_DEFAULT*/)
    {
        foundation::memory_globals::init();

        if (_enableMemTrack) {
            // TODO: add memory tracker
        }

        internal::init_platform();
    }

    void shutdown()
    {
        foundation::memory_globals::shutdown();

        internal::shutdown_platform();
    }

}