/*——————————————————————————————————————
	file: xpassthru.h
*/

#define MAX_BUNDLEID_LENGTH 50		
#define TAG 'ImPa'

/*——————————————————————————————————————
	预声明
*/
typedef struct _ADAPT ADAPT, *PADAPT;

extern
NTSTATUS
DriverEntry(
	IN	PDRIVER_OBJECT			DriverObject,
	IN	PUNICODE_STRING			RegistryPath
	);

#define DBGPRINT(Fmt)										\
	{														\
		DbgPrint("*** XPASSHTRU.SYS ***");					\
		DbgPrint (Fmt);										\
	}

/*——————————————————————————————————————
	Protocol 封包的保留部分
*/
typedef struct _ProtRsvd
{
	PNDIS_PACKET	OriginalPkt;
} RSVD, *PRSVD;

typedef enum 
{
	Passthru_Invalid,
	Passthru_SetPower,
	Passthru_Unbind

} PASSSTHRU_EVENT_CODE, *PPASTHRU_EVENT_CODE; 

typedef struct _PASSTHRU_EVENT
{
	NDIS_EVENT Event;
	PASSSTHRU_EVENT_CODE Code;

} PASSTHRU_EVENT, *PPASSTHRU_EVENT;


/*——————————————————————————————————————
	adapter 数据结构
*/
typedef struct _ADAPT
{
	PADAPT			Next;
	
	NDIS_HANDLE		BindingHandle;	// 指向底层miniport设备
	NDIS_HANDLE		MiniportHandle;	// 指向passthru的miniport设备
	NDIS_HANDLE		SendPacketPoolHandle;
	NDIS_HANDLE		RecvPacketPoolHandle;
	NDIS_STATUS		Status;			// 打开状态
	NDIS_EVENT		Event;			// 用来进行Open/Close的bind/halt操作同步
	NDIS_MEDIUM		Medium;
	NDIS_REQUEST	Request;		// 请求
	PULONG			BytesNeeded;
	PULONG			BytesReadOrWritten;
	BOOLEAN			IndicateRcvComplete;
	
	BOOLEAN			OutstandingRequests;  	
	BOOLEAN			QueuedRequest;		    
	BOOLEAN			StandingBy;				

	NDIS_DEVICE_POWER_STATE 	MPDeviceState;	// Miniport的设备状态 
	NDIS_DEVICE_POWER_STATE 	PTDeviceState;	// Protocol的设备状态 

	BOOLEAN			isSecondary;		// miniport是否是一个绑定的第二个设备
	NDIS_STRING		BundleUniString;	// 保存绑定ID
	PADAPT			pPrimaryAdapt;		// 指向主设备
	PADAPT			pSecondaryAdapt;	// 指向第二个设备
	KSPIN_LOCK		SpinLock;			// 同步锁

} ADAPT, *PADAPT;

extern	NDIS_PHYSICAL_ADDRESS			HighestAcceptableMax;
extern	NDIS_HANDLE						ProtHandle, DriverHandle;
extern	NDIS_MEDIUM						MediumArray[3];
extern	PADAPT							pAdaptList;
extern  NDIS_HANDLE						WrapperHandle;

/*——————————————————————————————————————
	自定义宏判断设备状态是否为ON
*/
#define IsIMDeviceStateOn(_pP)					\
	((_pP)->MPDeviceState == NdisDeviceStateD0	\
	&& (_pP)->PTDeviceState == NdisDeviceStateD0 ) 

