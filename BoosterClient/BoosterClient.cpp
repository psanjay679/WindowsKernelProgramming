#include <stdio.h>
#include <windows.h>
#include <winternl.h>


typedef struct _ThreadData {
    ULONG ThreadId;
    int Priority;
}ThreadData, *PThreadData;


int main()
{
    return 0;
}