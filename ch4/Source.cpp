#include "ntddk.h"

VOID DriverUnload(PDRIVER_OBJECT DriverObject) {
	
	UNREFERENCED_PARAMETER(DriverObject);
	KdPrint(("Driver Unload Successful"));

}

NTSTATUS BoosterCreateClose(PDEVICE_OBJECT DeviceObject, PIRP Irp);
NTSTATUS BoosterWrite(PDEVICE_OBJECT DeviceObject, PIRP Irp);

//BOOL WriteFile(
//	_In_ HANDLE hFile,
//	_In_reads_bytes_opt_(nNumberOfBytesToWrite) LPCVOID lpBuffer,
//	_In_ DWORD nNumberOfBytesToWrite,
//	_Out_opt_ LPDWORD lpNumberOfBytesWritten,
//	_Inout_opt_ LPOVERLAPPED lpOverlapped);

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {

	UNREFERENCED_PARAMETER(RegistryPath);

	KdPrint(("Driver Load Successful"));
	
	DriverObject->DriverUnload = DriverUnload;
	DriverObject->MajorFunction[IRP_MJ_CREATE] = BoosterCreateClose;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = BoosterCreateClose;
	DriverObject->MajorFunction[IRP_MJ_WRITE] = BoosterWrite;

	return STATUS_SUCCESS;

}

NTSTATUS BoosterCreateClose(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
	return STATUS_SUCCESS;
}

NTSTATUS BoosterWrite(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
	return STATUS_SUCCESS;
}
