#include <stdio.h>
#include <windows.h>
#include <winternl.h>


typedef struct _ThreadData {
    ULONG ThreadId;
    int Priority;
}ThreadData, *PThreadData;


int main(int argc, char *argv[])
{

    if (argc < 3) {
        printf("Usage: <%s> <thread_id> <priority>", argv[0]);
        exit(0);
    }

    int tid = atoi(argv[1]);
    int priority = atoi(argv[2]);

    HANDLE hFile;
    ThreadData tdata;

    hFile = CreateFile(L"\\\\.\\Booster", GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("error opening file. Error: (0x%08x)\n", GetLastError());
        exit(-1);
    }

    tdata.Priority = priority;
    tdata.ThreadId = tid;

    if (!WriteFile(hFile, &tdata, sizeof(tdata), NULL, nullptr)) {
        printf("Priority change failed. Error (0x%08x)\n", GetLastError());
        return -1;
    }

    CloseHandle(hFile);

    return 0;
}