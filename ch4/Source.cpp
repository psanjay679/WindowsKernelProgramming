#include "ntifs.h"
#include "ntddk.h"

typedef struct _ThreadData {
	ULONG ThreadId;
	int Priority;
} ThreadData, *PThreadData;

VOID DriverUnload(PDRIVER_OBJECT DriverObject) {
	
	UNREFERENCED_PARAMETER(DriverObject);
	KdPrint(("Driver Unload Successful"));

	UNICODE_STRING symLink = RTL_CONSTANT_STRING(L"\\??\\Booster");

	IoDeleteSymbolicLink(&symLink);
	IoDeleteDevice(DriverObject->DeviceObject);
}

NTSTATUS BoosterCreateClose(PDEVICE_OBJECT DeviceObject, PIRP Irp);
NTSTATUS BoosterWrite(PDEVICE_OBJECT DeviceObject, PIRP Irp);

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {

	UNREFERENCED_PARAMETER(RegistryPath);

	KdPrint(("Driver Load Successful"));
	
	NTSTATUS status = STATUS_SUCCESS;

	DriverObject->DriverUnload = DriverUnload;
	DriverObject->MajorFunction[IRP_MJ_CREATE] = BoosterCreateClose;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = BoosterCreateClose;
	DriverObject->MajorFunction[IRP_MJ_WRITE] = BoosterWrite;

	UNICODE_STRING devName = RTL_CONSTANT_STRING(L"\\Device\\Booster"), symLink;
	PDEVICE_OBJECT DeviceObject;

	status = IoCreateDevice(DriverObject, 0, &devName, FILE_DEVICE_UNKNOWN, 0, FALSE, &DeviceObject);

	if (!NT_SUCCESS(status)) {
		KdPrint(("Failed to create Device Object: (0x%08x)", status));
		return status;
	}

	symLink = RTL_CONSTANT_STRING(L"\\??\\Booster");

	status = IoCreateSymbolicLink(&symLink, &devName);

	if (!NT_SUCCESS(status)) {
		KdPrint(("Failed to create symlink: (0x%08x)\n", status));
		return status;
	}

	DriverObject->DeviceObject = DeviceObject;

	return status;

}

NTSTATUS BoosterCreateClose(PDEVICE_OBJECT DeviceObject, PIRP Irp) {

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}

NTSTATUS BoosterWrite(PDEVICE_OBJECT DeviceObject, PIRP Irp) {

	auto status = STATUS_SUCCESS;
	ULONG_PTR information = 0;
	PETHREAD Thread;

	auto irpSp = IoGetCurrentIrpStackLocation(Irp);

	do {

		if (irpSp->Parameters.Write.Length < sizeof(ThreadData)) {
			status = STATUS_BUFFER_TOO_SMALL;
			break;
		}

		auto data = static_cast<ThreadData*>(Irp->UserBuffer);

		if (data == nullptr || data->Priority < 1 || data->Priority > 31) {
			status = STATUS_INVALID_PARAMETER;
			break;
		}

		status = PsLookupThreadByThreadId(ULongToHandle(data->ThreadId), &Thread);

		if (!NT_SUCCESS(status)) {
			break;
		}

		auto oldPriority = KeSetPriorityThread(Thread, data->Priority);
		KdPrint(("Change of priority for thread: %u from: %u to %u done successfully\n", data->ThreadId, oldPriority, data->Priority));

		ObDereferenceObject(Thread);
		information = sizeof(data);
		Irp->IoStatus.Status = status;
		Irp->IoStatus.Information = information;
		IoCompleteRequest(Irp, IO_NO_INCREMENT);

	} while (false);

	return status;
}
