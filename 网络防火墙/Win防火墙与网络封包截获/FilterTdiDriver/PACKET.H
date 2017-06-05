///////////////////////////////////////////////////////////////////////
// Copyright (c) 2001-2002
// XStudio Technology All Right Reserved.
// Author: Tony Zhu
// 2001-7-10 Create
// summary:
//			This Program to demo the Intermediate TDI Driver.
///////////////////////////////////////////////////////////////////////
//Driver's Function
//——————————————————————————————————————
// DD_TCP_DEVICE_NAME	: 系统设备名称，我们将挂接自己设备到这个设备之上
// TDIH_TCP_DEVICE_NAME : 自己的设备名称
// TDIH_DEV_EXT_ATTACHED: 自己设备的扩展标志
//
#define		DD_TCP_DEVICE_NAME					L"\\Device\\Tcp"
#define		TDIH_TCP_DEVICE_NAME				L"\\Device\\TonyTcpFilter"
#define		TDIH_DEV_EXT_ATTACHED				(0x00000001)
/**************************************************************************
	每一个结构必须有一个唯一的“node type”或者一个联合签名
**************************************************************************/
#define		TDIH_NODE_TYPE_TCP_FILTER_DEVICE    (0xfdecba12)

/*
	输出调试信息
*/
#define DBGPRINT(Fmt)												\
{																	\
	DbgPrint(" ***FilterTdiDriver.sys*** ");						\
	DbgPrint (Fmt);													\
}

/*
	进行64位数值的处理，请参阅DDK帮助文档关于RtlLargeIntegerEqualToZero的解释
*/
#define UTIL_IsLargeIntegerZero(ReturnValue, LargeIntegerOp, pSpinLock)	\
{																		\
   KIRQL            OldIrql;                                            \
   KeAcquireSpinLock(pSpinLock, &OldIrql);                              \
   ASSERT(RtlLargeIntegerGreaterOrEqualToZero((LargeIntegerOp)));       \
   ReturnValue = RtlLargeIntegerEqualToZero((LargeIntegerOp));          \
   KeReleaseSpinLock(pSpinLock, OldIrql);                               \
}

//——————————————————————————————————————
// 用来保存驱动程序相关信息的自定义结构类型，这个结构类型可以绑定到
// DEVICE_OBJECT对象的DeviceExtension成员变量之上，随着DEVICE_OBJECT对象在
// 不同的函数之间传递
//
typedef struct _TDIH_DeviceExtension
{
	
	ULONG			NodeType;				// 标识这个结构	
	ULONG			NodeSize;				// 这个结构的大小
	PDEVICE_OBJECT	pFilterDeviceObject;	// 过滤设备对象
	KSPIN_LOCK		IoRequestsSpinLock;		// 同时调用时的保护锁
	KEVENT			IoInProgressEvent;		// 进程间同步处理
	ULONG			DeviceExtensionFlags;	// 设备标志
	PDEVICE_OBJECT	TargetDeviceObject;		// 绑定的设备对象
	PFILE_OBJECT    TargetFileObject;		// 绑定设备的文件对象
	PDEVICE_OBJECT  LowerDeviceObject;		// 绑定前底层设备对象
	LARGE_INTEGER   OutstandingIoRequests;	

}	TDIH_DeviceExtension, *PTDIH_DeviceExtension;

///////////////////////////////////////////////////////////////////////

NTSTATUS
DriverEntry(
	IN	PDRIVER_OBJECT		DriverObject,
	IN	PUNICODE_STRING		RegistryPath
);

VOID 
PacketUnload(
	IN PDRIVER_OBJECT		DriverObject
);

NTSTATUS
PacketDispatch(
    IN PDEVICE_OBJECT		DeviceObject,
    IN PIRP					Irp
);

NTSTATUS
PacketCompletion(
	IN	PDEVICE_OBJECT	DeviceObject,
	IN	PIRP			Irp,
	IN	PVOID			Context
);

NTSTATUS
TCPFilter_Attach(
	IN PDRIVER_OBJECT	DriverObject,
	IN PUNICODE_STRING	RegistryPath
);

NTSTATUS
TCPFilter_InitDeviceExtension(
	IN	PTDIH_DeviceExtension	pTDIH_DeviceExtension,
	IN	PDEVICE_OBJECT			pFilterDeviceObject,
	IN	PDEVICE_OBJECT			pTargetDeviceObject,
	IN	PFILE_OBJECT			pTargetFileObject,
	IN	PDEVICE_OBJECT			pLowerDeviceObject
);

VOID
TCPFilter_Detach(
   IN	PDEVICE_OBJECT pDeviceObject
);

