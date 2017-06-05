///////////////////////////////////////////////////////////////////////
// Copyright (c) 2001-2002
// XStudio Technology All Right Reserved.
// Author: Tony Zhu
// 2001-7-16 Create
// summary:
//			This Program to demo the Intermediate TDI Driver.
//			This is main file. It include:
//			1. initialize function	: DriverEntry
//			2. Unload function		: PacketUnload
//			3. Dispatch function	: PacketDispatch
///////////////////////////////////////////////////////////////////////

#include <ndis.h>
#include <tdikrnl.h>
#include <ntddk.h>
#include "packet.h"

NTSTATUS
DriverEntry(
	IN	PDRIVER_OBJECT		DriverObject,
	IN	PUNICODE_STRING		RegistryPath
)
{
	NTSTATUS	status	= 0;
    ULONG		i;

	DBGPRINT("DriverEntry Loading...\n");
	DriverObject->DriverUnload = PacketUnload;

    for (i=0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++)
    {
		DriverObject->MajorFunction[i] = PacketDispatch;
    }

	status = TCPFilter_Attach(DriverObject,RegistryPath);

	return status;
}

VOID 
PacketUnload(
	IN PDRIVER_OBJECT		DriverObject
)
{
    PDEVICE_OBJECT			DeviceObject;
    PDEVICE_OBJECT			OldDeviceObject;
	PTDIH_DeviceExtension	pTDIH_DeviceExtension;	

 	DBGPRINT("DriverEntry unLoading...\n");

	DeviceObject = DriverObject->DeviceObject;

   while (DeviceObject != NULL) 
	{
        OldDeviceObject = DeviceObject;
		pTDIH_DeviceExtension	
			= (PTDIH_DeviceExtension )DeviceObject->DeviceExtension;
		if( pTDIH_DeviceExtension->NodeType 
			== TDIH_NODE_TYPE_TCP_FILTER_DEVICE )
			TCPFilter_Detach( DeviceObject );   // Calls IoDeleteDevice
		else
			IoDeleteDevice(OldDeviceObject);
        DeviceObject = DeviceObject->NextDevice;
    }
}

NTSTATUS
PacketDispatch(
    IN PDEVICE_OBJECT		DeviceObject,
    IN PIRP					Irp
)
{
	NTSTATUS				RC = STATUS_SUCCESS;
	PTDIH_DeviceExtension	pTDIH_DeviceExtension;
	PIO_STACK_LOCATION		IrpStack;
	PIO_STACK_LOCATION		NextIrpStack;

	pTDIH_DeviceExtension	
		= (PTDIH_DeviceExtension )(DeviceObject->DeviceExtension);

	IrpStack = IoGetCurrentIrpStackLocation(Irp);

	switch(IrpStack->MajorFunction)
	{
	case IRP_MJ_CREATE:
	 	DBGPRINT("PacketDispatch(IRP_MJ_CREATE)...\n");
		break;
	case IRP_MJ_CLOSE:
	 	DBGPRINT("PacketDispatch(IRP_MJ_CLOSE)...\n");
		break;
	case IRP_MJ_CLEANUP:
	 	DBGPRINT("PacketDispatch(IRP_MJ_CLEANUP)...\n");
		break;
	case IRP_MJ_INTERNAL_DEVICE_CONTROL:
		switch (IrpStack->MinorFunction) 
		{
		case TDI_ACCEPT:
		 	DBGPRINT("PacketDispatch(IRP_MJ_INTERNAL_DEVICE_CONTROL\
				[TDI_ACCEPT])...\n");
			break;
		case TDI_ACTION:
		 	DBGPRINT("PacketDispatch(IRP_MJ_INTERNAL_DEVICE_CONTROL\
				[TDI_ACTION])...\n");
			break;
		case TDI_ASSOCIATE_ADDRESS:
		 	DBGPRINT("PacketDispatch(IRP_MJ_INTERNAL_DEVICE_CONTROL\
				[TDI_ASSOCIATE_ADDRESS])...\n");
			break;
		case TDI_DISASSOCIATE_ADDRESS:
		 	DBGPRINT("PacketDispatch(IRP_MJ_INTERNAL_DEVICE_CONTROL\
				[TDI_DISASSOCIATE_ADDRESS])...\n");
			break;
		case TDI_CONNECT:
		 	DBGPRINT("PacketDispatch(IRP_MJ_INTERNAL_DEVICE_CONTROL\
				[TDI_CONNECT])...\n");
			break;
		case TDI_DISCONNECT:
		 	DBGPRINT("PacketDispatch(IRP_MJ_INTERNAL_DEVICE_CONTROL\
				[TDI_DISCONNECT])...\n");
			break;
		case TDI_LISTEN:
		 	DBGPRINT("PacketDispatch(IRP_MJ_INTERNAL_DEVICE_CONTROL\
				[TDI_LISTEN])...\n");
			break;
		case TDI_QUERY_INFORMATION:
		 	DBGPRINT("PacketDispatch(IRP_MJ_INTERNAL_DEVICE_CONTROL\
				[TDI_QUERY_INFORMATION])...\n");
			break;
		case TDI_RECEIVE:
		 	DBGPRINT("PacketDispatch(IRP_MJ_INTERNAL_DEVICE_CONTROL\
				[TDI_RECEIVE])...\n");
			break;
		case TDI_RECEIVE_DATAGRAM:
		 	DBGPRINT("PacketDispatch(IRP_MJ_INTERNAL_DEVICE_CONTROL\
				[TDI_RECEIVE_DATAGRAM])...\n");
			break;
		case TDI_SEND:
		 	DBGPRINT("PacketDispatch(IRP_MJ_INTERNAL_DEVICE_CONTROL\
				[TDI_SEND])...\n");
			break;
		case TDI_SEND_DATAGRAM:
		 	DBGPRINT("PacketDispatch(IRP_MJ_INTERNAL_DEVICE_CONTROL\
				[TDI_SEND_DATAGRAM])...\n");
			break;
		case TDI_SET_EVENT_HANDLER:
		 	DBGPRINT("PacketDispatch(IRP_MJ_INTERNAL_DEVICE_CONTROL\
				[TDI_SET_EVENT_HANDLER])...\n");
			break;
		case TDI_SET_INFORMATION:
		 	DBGPRINT("PacketDispatch(IRP_MJ_INTERNAL_DEVICE_CONTROL\
				[TDI_SET_INFORMATION])...\n");
			break;
		default:
		 	DBGPRINT("PacketDispatch(IRP_MJ_INTERNAL_DEVICE_CONTROL\
				[INVALID_MINOR_FUNCTION])...\n");
			break;
		}
		break;
	case IRP_MJ_DEVICE_CONTROL:
		DBGPRINT("PacketDispatch(IRP_MJ_DEVICE_CONTROL)...\n");
		break;
	default:
		DBGPRINT("PacketDispatch(OTHER_MAJOR_FUNCTION)...\n");
		break;
	}

	if (Irp->CurrentLocation == 1)
	{
		ULONG ReturnedInformation = 0;

		DBGPRINT(("PacketDispatch encountered bogus current location\n"));

		RC = STATUS_INVALID_DEVICE_REQUEST;
		Irp->IoStatus.Status = RC;
		Irp->IoStatus.Information = ReturnedInformation;
		IoCompleteRequest(Irp, IO_NO_INCREMENT);

		return( RC );
	}

	NextIrpStack	= IoGetNextIrpStackLocation(Irp);
	*NextIrpStack	= *IrpStack;

	IoSetCompletionRoutine(Irp,PacketCompletion,NULL,TRUE,TRUE,TRUE);

	return IoCallDriver(pTDIH_DeviceExtension->LowerDeviceObject,Irp);
}

NTSTATUS
PacketCompletion(
	IN	PDEVICE_OBJECT	DeviceObject,
	IN	PIRP			Irp,
	IN	PVOID			Context
)
{
	if(Irp->PendingReturned)
		IoMarkIrpPending(Irp);

	return STATUS_SUCCESS;
}

NTSTATUS
TCPFilter_Attach(
	IN PDRIVER_OBJECT	DriverObject,
	IN PUNICODE_STRING	RegistryPath
)
{
	NTSTATUS				status	= 0;
	UNICODE_STRING			uniNtNameString;
	PTDIH_DeviceExtension	pTDIH_DeviceExtension;
	PDEVICE_OBJECT			pFilterDeviceObject = NULL;
	PDEVICE_OBJECT			pTargetDeviceObject = NULL;
	PFILE_OBJECT			pTargetFileObject	= NULL;
	PDEVICE_OBJECT			pLowerDeviceObject	= NULL;

	DBGPRINT("TCPFilter_Attach.\n");

	RtlInitUnicodeString( &uniNtNameString, DD_TCP_DEVICE_NAME );

	status = IoGetDeviceObjectPointer(
               IN	&uniNtNameString,
               IN	FILE_READ_ATTRIBUTES,
               OUT	&pTargetFileObject,   
               OUT	&pTargetDeviceObject
               );
	if( !NT_SUCCESS(status) )
	{
		DBGPRINT(("TCPFilter_Attach: Couldn't get the TCP Device Object\n"));
		pTargetFileObject	= NULL;
		pTargetDeviceObject = NULL;
		return( status );
	}

	RtlInitUnicodeString( &uniNtNameString, TDIH_TCP_DEVICE_NAME );

	status = IoCreateDevice(
               IN	DriverObject,
               IN	sizeof( TDIH_DeviceExtension ),
               IN	&uniNtNameString,
               IN	pTargetDeviceObject->DeviceType,
               IN	pTargetDeviceObject->Characteristics,
               IN	FALSE,                 
               OUT	&pFilterDeviceObject
               );
	if( !NT_SUCCESS(status) )
	{
		DBGPRINT(("TCPFilter_Attach: Couldn't create the TCP Filter Device Object\n"));
		ObDereferenceObject( pTargetFileObject );
		pTargetFileObject = NULL;
		pTargetDeviceObject = NULL;
		return( status );
	}

	pLowerDeviceObject 
		= IoAttachDeviceToDeviceStack(pFilterDeviceObject,pTargetDeviceObject);
	if( !pLowerDeviceObject )
	{
		DBGPRINT(("TCPFilter_Attach: Couldn't attach to TCP Device Object\n"));
		IoDeleteDevice( pFilterDeviceObject );
		pFilterDeviceObject = NULL;
		ObDereferenceObject( pTargetFileObject );
		pTargetFileObject	= NULL;
		pTargetDeviceObject = NULL;
		return( status );
	}

	pTDIH_DeviceExtension 
		= (PTDIH_DeviceExtension )( pFilterDeviceObject->DeviceExtension );
	TCPFilter_InitDeviceExtension(
		IN	pTDIH_DeviceExtension,
		IN	pFilterDeviceObject,
		IN	pTargetDeviceObject,
		IN	pTargetFileObject,
		IN	pLowerDeviceObject
		);

	pFilterDeviceObject->Flags |= pTargetDeviceObject->Flags 
		& (DO_BUFFERED_IO | DO_DIRECT_IO);
	return status;
}

NTSTATUS
TCPFilter_InitDeviceExtension(
	IN	PTDIH_DeviceExtension	pTDIH_DeviceExtension,
	IN	PDEVICE_OBJECT			pFilterDeviceObject,
	IN	PDEVICE_OBJECT			pTargetDeviceObject,
	IN	PFILE_OBJECT			pTargetFileObject,
	IN	PDEVICE_OBJECT			pLowerDeviceObject
)
{
	NdisZeroMemory( pTDIH_DeviceExtension, sizeof( TDIH_DeviceExtension ) );
	pTDIH_DeviceExtension->NodeType	= TDIH_NODE_TYPE_TCP_FILTER_DEVICE;
	pTDIH_DeviceExtension->NodeSize	= sizeof( TDIH_DeviceExtension );
	pTDIH_DeviceExtension->pFilterDeviceObject = pFilterDeviceObject;
	KeInitializeSpinLock(&(pTDIH_DeviceExtension->IoRequestsSpinLock));
	KeInitializeEvent(&(pTDIH_DeviceExtension->IoInProgressEvent)
		, NotificationEvent, FALSE);
	pTDIH_DeviceExtension->TargetDeviceObject	= pTargetDeviceObject;
	pTDIH_DeviceExtension->TargetFileObject		= pTargetFileObject;
	pTDIH_DeviceExtension->LowerDeviceObject	= pLowerDeviceObject;
	pTDIH_DeviceExtension->DeviceExtensionFlags |= TDIH_DEV_EXT_ATTACHED;
	return( STATUS_SUCCESS );
}

VOID
TCPFilter_Detach(
	IN	PDEVICE_OBJECT pDeviceObject
)
{
	PTDIH_DeviceExtension	pTDIH_DeviceExtension;
	BOOLEAN					NoRequestsOutstanding = FALSE;

	pTDIH_DeviceExtension 
		= (PTDIH_DeviceExtension )pDeviceObject->DeviceExtension;
	try
	{
		try
		{
			while (TRUE)
			{
				UTIL_IsLargeIntegerZero(
					NoRequestsOutstanding,
					pTDIH_DeviceExtension->OutstandingIoRequests,
					&(pTDIH_DeviceExtension->IoRequestsSpinLock)
					);
				if( !NoRequestsOutstanding )
					KeWaitForSingleObject(
					(void *)(&(pTDIH_DeviceExtension->IoInProgressEvent)),
					Executive, KernelMode, FALSE, NULL
					);
				else
					break;
			}

			if( pTDIH_DeviceExtension->DeviceExtensionFlags 
				& TDIH_DEV_EXT_ATTACHED)
			{
				IoDetachDevice( pTDIH_DeviceExtension->TargetDeviceObject );
				pTDIH_DeviceExtension->DeviceExtensionFlags 
					&= ~(TDIH_DEV_EXT_ATTACHED);
			}

			pTDIH_DeviceExtension->NodeType = 0;
			pTDIH_DeviceExtension->NodeSize = 0;
			if( pTDIH_DeviceExtension->TargetFileObject )
				ObDereferenceObject( pTDIH_DeviceExtension->TargetFileObject );
			pTDIH_DeviceExtension->TargetFileObject = NULL;

			IoDeleteDevice( pDeviceObject );

			DBGPRINT(("TCPFilter_Attach: TCPFilter_Detach Finished\n"));
		}
		except (EXCEPTION_EXECUTE_HANDLER){}
	}
	finally{}
	return;
}

