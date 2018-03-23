#include "global_init.h"

#include <signal.h>

#include "runnable_pool.h"

namespace gamed {
namespace globalInit{
static const int kRunnablePoolThreadNum = 8;

namespace {
    void sigbus_handler(int)
    {
        _exit(0);
    }

    int InitSignalProcMask()
    {
        // SIG_BLOCK
        sigset_t set;
        sigemptyset(&set);
        sigaddset(&set, SIGALRM);

        // SIGBUS
        struct sigaction act;
        act.sa_handler = sigbus_handler;
        sigemptyset(&act.sa_mask);
        act.sa_flags = 0;
        sigaction(SIGBUS, &act, 0);

        return 0;
    }

} // Anonymous

int InitGameServer()
{
}

} // globalInit
} // gamed
