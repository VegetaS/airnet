#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <google/protobuf/stubs/common.h>
#include "global_init.h"
using namespace std;

static bool done = false;

void int_handler(int no) {
    printf("\nBye\n");
    done = true;
}

void usage(const char *name) { printf("Usage: %s [-v] [-h] [configurefile]\n", name); }

int main(int argc, char *argv[]) {

    /// signal handle
    ///
    ///
    signal(SIGINT, int_handler);


    ///
    /// touch file system
    ///
    if (system("/bin/touch foo")) {
        printf("文件系统不可写，无法进行后继的初始化操作\n");
        return -1;
    }

    ///
    /// option parse
    ///
    int opt;
    while ((opt = getopt(argc, argv, "hv")) != EOF) {
        switch (opt) {
            case 'v': printf("%s\n", "??????? version"); exit(0);
            default: usage(argv[0]); exit(0);
        }
    }
    
    int ret = globalInit::InitServer();
    if (ret)
    {
        printf("Init Error %d\n", ret);
        exit(ret);
    }

    ///
    /// main thread wait
    ///
    while(!done)
    {
        sleep(1);
    }

    ///
    /// stop server
    ///
    ret = globalInit::StopServer();
    if (ret)
    {
        printf("Stop Server error: %d\n", ret);
        exit(ret);
    }

    ///
    /// exit program
    ///
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
