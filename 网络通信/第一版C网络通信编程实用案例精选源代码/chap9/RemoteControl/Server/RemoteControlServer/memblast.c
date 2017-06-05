#include "memblast.h"
//#include "Server.h"
//#include "HuffCompress.h"
#include <windows.h>

// Copy memory range from one location to another
__inline void memblast(void* dest,void* src,DWORD count)
{
	DWORD	iCount;

	__asm
	{
		// Align Count to a DWORD Boundary
		MOV		ECX,count
		SHR		ECX,2
		SHL		ECX,2
		MOV		iCount,ECX

		// Copy All the DWORDs (32 bits at a Time)
		MOV		ESI,src		// Copy the Source Address to the Register
		MOV		EDI,dest	// Copy the Destination to the Register
		MOV		ECX,iCount	// Copy the Count to the Register
		SHR		ECX,2		// Divide Count by 4 for DWORD Copy
		REP		MOVSD		// Move all the Source DWORDs to the Dest DWORDs

		// Get the Remaining Bytes to Copy
		MOV		ECX,count
		MOV		EAX,iCount
		SUB		ECX,EAX

		// Exit if All Bytes Copied
		JZ		Exit

		// Copy the Remaining BYTEs (8 bits at a Time)
		MOV		ESI,src		// Copy the Source Address to the Register
		ADD		ESI,EAX		// Set the Starting Point of the Copy
		MOV		EDI,dest	// Copy the Destination to the Register
		ADD		EDI,EAX		// Set the Destination Point of the Copy
		REP		MOVSB		// Move all the Source BYTEs to the Dest BYTEs
		Exit:
	}
}