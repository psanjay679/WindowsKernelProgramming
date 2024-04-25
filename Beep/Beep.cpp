// Beep.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <winternl.h>
#include <stdio.h>
#include <ntddbeep.h>

#pragma comment(lib, "ntdll")

void do_beep(int frequency, int duration) {
    
    HANDLE hFile;
    OBJECT_ATTRIBUTES attr;
    UNICODE_STRING name;
    IO_STATUS_BLOCK ioStatus;
    NTSTATUS status;

    RtlInitUnicodeString(&name, L"\\Device\\Beep");

    for (int i = 0; i < 10; i++) {

        InitializeObjectAttributes(&attr, &name, OBJ_CASE_INSENSITIVE, nullptr, nullptr);

        status = ::NtOpenFile(&hFile, GENERIC_WRITE, &attr, &ioStatus, 0, 0);

        if (NT_SUCCESS(status)) {
            BEEP_SET_PARAMETERS params;
            DWORD bytes;

            params.Frequency = frequency + (i + 1) * 10000000;
            params.Duration = duration;

            printf("Playing sound freq: %u\n", params.Frequency, duration);
            ::DeviceIoControl(hFile, IOCTL_BEEP_SET, &params, sizeof(params), nullptr, 0, &bytes, nullptr);

            printf("Sleeping for duration: %u\n", duration);
            ::Sleep(duration);
            ::CloseHandle(hFile);

        }
    }

}

int main(int argc, char *argv[])
{
    printf("Usage <%s> [<frequency> <duration_in_msec>]\n", argv[0]);
    int frequency, duration;

    if (argc < 2) {
        frequency = 500;
        duration = 10000;
    }

    if (argc >= 2) {
        frequency = atoi(argv[1]);
    }

    if (argc >= 3) {
        duration = atoi(argv[2]);
    }

    do_beep(frequency, duration);

    return 0;

}
