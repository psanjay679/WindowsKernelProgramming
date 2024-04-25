#include "ntddk.h"

void DriverUnload(PDRIVER_OBJECT DriverObject) {

	UNREFERENCED_PARAMETER(DriverObject);
	KdPrint(("Driver Unloaded Successfully"));
}

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {

	UNREFERENCED_PARAMETER(RegistryPath);
	DriverObject->DriverUnload = DriverUnload;

	KdPrint(("Driver Loaded Successfully"));

	return STATUS_SUCCESS;

}