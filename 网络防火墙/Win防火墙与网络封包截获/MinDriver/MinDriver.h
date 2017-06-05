//！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
// Copyright (c) 2001-2002
// XStudio Technology All Right Reserved.
// Author: Tony Zhu
// 2001-7-10 Create
// summary:
//			This Program to demo the Intermediate Miniport NDIS Driver.
//！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
// 補竃距編佚連崎
//
#define DBGPRINT(Fmt)												\
{																	\
	DbgPrint("MinDriver debug: %s (%d)", __FILE__, __LINE__);		\
	DbgPrint (Fmt);													\
}

//！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
//Driver's Function
//！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
// 駁強殻會秘笥痕方
//
NTSTATUS
DriverEntry(
	IN	PDRIVER_OBJECT		DriverObject,
	IN	PUNICODE_STRING		RegistryPath
);

//！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
// 駁強殻會亢墮痕方
//
VOID 
PacketUnload(
	IN PDRIVER_OBJECT		DriverObject
);

