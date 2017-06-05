// Run Length ±àÂë½âÂëº¯Êý¼¯
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "RLE.h"
#include "Server.h"

DWORD	dwCodeLength = 19;

DWORD RunLength(BYTE *pInput,DWORD dwCount)
{
	DWORD	dwNewCount = 4;
	DWORD	dwMaxRun;
	DWORD	dwUnusedByte = 0;
	BOOL	fOutput = FALSE;

	__asm
	{
		MOV		EDX,1
		MOV		ECX,dwCodeLength
		SUB		ECX,9
		SHL		EDX,CL
		SUB		EDX,1
		MOV		dwMaxRun,EDX

		MOV		EDX,dwCount

		XOR		EDI,EDI

		XOR		ESI,ESI

		_LoadByte:

		PUSH	ESI

		MOV		ESI,pInput
		MOV		AL,[ESI]

		MOV		ECX,DWORD PTR 1

		_CompareBytes:

		CMP		EDX,0
		JZ		_Finished

		CMP		ECX,dwMaxRun
		JA		_Finished

		CMP		AL,[ESI + ECX]

		LEA		ECX,[ECX + 1]

		LEA		EDX,[EDX - 1]

		JE		_CompareBytes

		_Finished:

		DEC		ECX

		ADD		ESI,ECX
		MOV		pInput,ESI

		POP		ESI

		CMP		ECX,1
		JNZ		_BuildCode

		XOR		EBX,EBX
		MOV		BH,1
		MOV		BL,AL

		MOV		ECX,9

		JMP		_CheckShift

		_BuildCode:
		MOV		EBX,ECX
		SHL		EBX,8
		MOV		BL,AL

		MOV		ECX,dwCodeLength

		_CheckShift:
		ADD		EDI,ECX
		CMP		EDI,32
		JLE		_ShiftData

		SUB		EDI,32

		SUB		ECX,EDI

		MOV		dwUnusedByte,EBX

		PUSH	ECX
		MOV		ECX,EDI
		SHR		EBX,CL
		POP		ECX

		// Set the Output Flag
		MOV		fOutput,TRUE

		// Shift the Output Data
		_ShiftData:

		// Shift the Output Symbol to Handle a New Symbol
		SHL		ESI,CL

		// OR the Output Symbol with New Symbol
		OR		ESI,EBX

		// Check for Outputting a full DWORD
		CMP		fOutput,FALSE
		JE		_Continue

		// Increment the Compression Count
		ADD		dwNewCount,4

		// Remove the Bits we Used
		MOV		EAX,32
		SUB		EAX,EDI
		MOV		ECX,EAX

		// Restore the Unused Portion of the Code
		MOV		EAX,dwUnusedByte
		SHL		EAX,CL
		SHR		EAX,CL

		// Update the Code to Have the Left Over Bits
		MOV		ESI,EAX

		// Initialize the Left Over Bits
		MOV		dwUnusedByte,0

		// Reset the Output Flag
		MOV		fOutput,FALSE

		// Compare for More Data
		_Continue:
		CMP		EDX,0
		JZ		_Exit

		// Go Back for More Bytes
		JMP		_LoadByte

		// Finished Encoding
		_Exit:

		// Update Any Remaining Compression data in the Buffer
		CMP		EDI,0
		JE		_Done

		// Left Shift the Remaining Bits
		MOV		EAX,32
		SUB		EAX,EDI
		MOV		ECX,EAX
		SHL		ESI,CL

		// Increment the Compression Count
		ADD		dwNewCount,4

		// Done Encoding
		_Done:
	}

	// Return the Compressed Count
	return dwNewCount;
}

// Run Length Encode the Input Data to the Output Data
DWORD RunLengthEncode(BYTE *pInput,DWORD dwCount,BYTE *pOutput)
{
	// Compressed Count
	DWORD	dwNewCount = 4;

	// Variables Used for Code Shifting
	DWORD	dwUnusedByte = 0;
	BOOL	fOutput = FALSE;

	// Max Run Length
	DWORD	dwMaxRun;

	__asm
	{
		// Calculate the Maximum Run Length
		MOV		EDX,1
		MOV		ECX,dwCodeLength
		SUB		ECX,9
		SHL		EDX,CL
		SUB		EDX,1
		MOV		dwMaxRun,EDX

		// Point to the Input Byte
		MOV		EDI,pOutput

		// Load the Count of Bytes to Encode
		MOV		EDX,dwCount

		// Store the Original Length in the Compressed Data
		MOV		[EDI],EDX

		// Increment the Input Data Pointer
		ADD		EDI,4
		MOV		pOutput,EDI

		// Initialize the Output Bits
		XOR		EDI,EDI

		// Initialize the Storage DWORD
		XOR		ESI,ESI

		// Load the Byte to Compare
		_LoadByte:

		// Save the Storage DWORD
		PUSH	ESI

		// Load a Byte
		MOV		ESI,pInput
		MOV		AL,[ESI]

		// Initialize the Run Length Count to 1
		MOV		ECX,DWORD PTR 1

		// Compare the Bytes
		_CompareBytes:

		// Check for EOD
		CMP		EDX,0
		JZ		_Finished

		// Compare to the Max Length for an Encode
		CMP		ECX,dwMaxRun
		JA		_Finished

		// Compare the Bytes
		CMP		AL,[ESI + ECX]

		// Increment the Run Length
		LEA		ECX,[ECX + 1]

		// Decrement the Bytes Processed
		LEA		EDX,[EDX - 1]

		// Branch for the Comparison
		JE		_CompareBytes

		// Finished Encoding
		_Finished:

		// Decrement the Run Length for Encoding
		DEC		ECX

		// Increment the Pointer to the Source Data
		ADD		ESI,ECX
		MOV		pInput,ESI

		// Restore the Storage DWORD
		POP		ESI

		// Check for a 1:1 Character Copy
		CMP		ECX,1
		JNZ		_BuildCode

		// Move in the Character
		XOR		EBX,EBX
		MOV		BH,1
		MOV		BL,AL

		// Set the Code Length
		MOV		ECX,9

		// Jump to Checking the Bit Shifting
		JMP		_CheckShift

		// Build the Compression Code
		_BuildCode:
		MOV		EBX,ECX
		SHL		EBX,8
		MOV		BL,AL

		// Set the Code Length
		MOV		ECX,dwCodeLength

		// See How Many Bits Can be Shifted to the Output DWORD
		_CheckShift:
		ADD		EDI,ECX
		CMP		EDI,32
		JLE		_ShiftData

		// See How Many Bits of the Last Code Can't Be Used
		SUB		EDI,32

		// Adjust ECX to Handle only the Bits that Can Be Used
		SUB		ECX,EDI

		// Save the Code
		MOV		dwUnusedByte,EBX

		// Shift the Input Symbol to the Right
		PUSH	ECX
		MOV		ECX,EDI
		SHR		EBX,CL
		POP		ECX

		// Set the Output Flag
		MOV		fOutput,TRUE

		// Shift the Output Data
		_ShiftData:

		// Shift the Output Symbol to Handle a New Symbol
		SHL		ESI,CL

		// OR the Output Symbol with New Symbol
		OR		ESI,EBX

		// Check for Outputting a full DWORD
		CMP		fOutput,FALSE
		JE		_Continue

		// Output an Encoded Symbol
		MOV		EAX,ESI
		PUSH	EDI
		MOV		EDI,pOutput
		MOV		[EDI],EAX
		ADD		EDI,4
		MOV		pOutput,EDI
		POP		EDI

		// Increment the Compression Count
		ADD		dwNewCount,4

		// Remove the Bits we Used
		MOV		EAX,32
		SUB		EAX,EDI
		MOV		ECX,EAX

		// Restore the Unused Portion of the Code
		MOV		EAX,dwUnusedByte
		SHL		EAX,CL
		SHR		EAX,CL

		// Update the Code to Have the Left Over Bits
		MOV		ESI,EAX

		// Initialize the Left Over Bits
		MOV		dwUnusedByte,0

		// Reset the Output Flag
		MOV		fOutput,FALSE

		// Compare for More Data
		_Continue:
		CMP		EDX,0
		JZ		_Exit

		// Go Back for More Bytes
		JMP		_LoadByte

		// Finished Encoding
		_Exit:

		// Update Any Remaining Compression data in the Buffer
		CMP		EDI,0
		JE		_Done

		// Left Shift the Remaining Bits
		MOV		EAX,32
		SUB		EAX,EDI
		MOV		ECX,EAX
		SHL		ESI,CL

		// Output the Final Encoded Symbol
		MOV		EAX,ESI
		MOV		EDI,pOutput
		MOV		[EDI],EAX

		// Increment the Compression Count
		ADD		dwNewCount,4

		// Done Encoding
		_Done:
	}

	// Return the Compressed Count
	return dwNewCount;
}

// Run Length Decode the Input Data to the Output Data
DWORD RunLengthDecode(BYTE *pInput,BYTE *pOutput)
{
	// Compressed Count
	DWORD	dwCount;
	DWORD	dwCompCount;
	DWORD	dwCodeLength2;

	__asm
	{
		// Get the CodeLength - 1 for Checking a 1:1 Compression
		MOV		EAX,dwCodeLength
		DEC		EAX
		MOV		dwCodeLength2,EAX

		// Point to the Input Byte
		MOV		ESI,pInput

		// Point to the Output Byte
		MOV		EDI,pOutput

		// Load the Count of Bytes Encoded
		LODSD

		// Save the New Input Location
		MOV		pInput,ESI

		// Save the Count of the UnCompressed Data
		MOV		dwCompCount,EAX

		// Initialize the Count of Bytes Decoded
		MOV		dwCount,EAX

		// Initialize Number of Bits for Current Symbol to Process
		MOV		ESI,dwCodeLength

		// Initialize the Decode Storage
		XOR		EDX,EDX

		// Main Loop for Reading Input
		_LoopInput:

		// Save the Number of Bits for the Current Symbol
		PUSH	ESI

		// Point to the Input Data
		MOV		ESI,pInput

		// Read an Encoded DWORD
		LODSD

		// Save the Pointer to the Input Data
		MOV		pInput,ESI

		// Restore the Number of Bits for the Current Symbol
		POP		ESI

		// Initialize Number of Bits to Process
		MOV		EBX,32

		// Shift in an Encoded Symbol, 1 bit at a time
		_LeftShift:

		// Shift 1 Bit of the Input Code to the Output
		SHLD	EDX,EAX,1

		// Shift the Input Code 1 Bit to the Left
		SHL		EAX,1

		// Decrement the Number of Bits Processed
		DEC		ESI

		// Check for a Fully Encoded Symbol
		JZ		_Output

		// Compare the High Bit for RLE or Character Encode
		CMP		ESI,dwCodeLength2
		JNZ		_CheckBits

		// Compare the High Bit, 0 = RLE, 1 = 1:1 Character Encode
		CMP		EDX,0
		JZ		_CheckBits

		// Update the Bits Left to 8
		MOV		ESI,8

		// Check for More Bits to Process
		_CheckBits:

		// Decrement the Number of Bits Processed in the Input DWORD
		DEC		EBX

		// Continue Shifting More Bits
		JNZ		_LeftShift

		// Read Another DWORD of Data
		JMP		_LoopInput

		// Output the Run Length Amount of the Code
		_Output:

		// Get the Run Length
		MOV		ECX,EDX
		SHR		ECX,8

		// Get the Code
		AND		EDX,255

		// Save the Input DWORD
		PUSH	EAX

		// Copy the Code
		MOV		EAX,EDX

		// Save the Run Length
		PUSH	ECX

		// Output the Run Length Amount of the Code
		REP		STOSB

		// Restore the Run Length
		POP		ECX

		// Restore the Input DWORD
		POP		EAX

		// Check for More Data to Decode
		SUB		dwCompCount,ECX
		JZ		_Exit

		// Initialize the Decode Storage
		XOR		EDX,EDX

		// Initialize Number of Bits for Current Symbol to Process
		MOV		ESI,dwCodeLength

		// Check for More Bits to Process
		JMP		_CheckBits

		// Finished
		_Exit:
	}

	// Return the Count of Uncompressed Data
	return dwCount;
}