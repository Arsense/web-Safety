///////////////////////////////////////////////////////////////////////
// Copyright (c) 2001-2002
// XStudio Technology All Right Reserved.
// Author: Tony Zhu
// 2001-7-10 Create
// summary:
// This Program to demo the Intermediate Miniport NDIS Driver.
///////////////////////////////////////////////////////////////////////

#include <ndis.h>
#include "MinDriver.h"

NTSTATUS
DriverEntry(
	IN	PDRIVER_OBJECT		DriverObject,
	IN	PUNICODE_STRING		RegistryPath
)
{
	DBGPRINT("DriverEntry Loading...\n");

	DriverObject->DriverUnload = PacketUnload;

	return(0);
}

VOID 
PacketUnload(
	IN PDRIVER_OBJECT		DriverObject
)
{
    PDEVICE_OBJECT     DeviceObject;
    PDEVICE_OBJECT     OldDeviceObject;
		
 	DBGPRINT("DriverEntry unLoading...\n");

	DeviceObject = DriverObject->DeviceObject;

    while (DeviceObject != NULL) 
	{
        OldDeviceObject	= DeviceObject;
        DeviceObject	= DeviceObject->NextDevice;
        IoDeleteDevice(OldDeviceObject);
    }
}



