#include "mylogging.h"

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include "mylastheader.h"

static void fatal_usage() {
    fprintf(stderr, "Usage: progname PID\n");
    _exit(1);
}

int main(int argc, char *argv[])
{
    DWORD pid;
    DWORD exitCode;
    HANDLE hProcess;

    if (argc != 2) {
        fatal_usage();
    }
    if (1 != sscanf_s(argv[1], "%u", &pid)) {
        fatal_usage();
    }

    setlocale(LC_ALL, ".ACP");

    hProcess = OpenProcess(SYNCHRONIZE | PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
    if (!hProcess) {
        pWin32Error(ERR, "OpenProcess() failed");
        _exit(1);
    }

    if (WAIT_OBJECT_0 != WaitForSingleObject(hProcess, INFINITE)) {
        pWin32Error(ERR, "WaitForSingleObject() failed");
        _exit(1);
    }

    if (!GetExitCodeProcess(hProcess, &exitCode)) {
        pWin32Error(ERR, "GetExitCodeProcess() failed");
        _exit(1);
    }
    printf("%u\n", exitCode);
    return 0;
}
