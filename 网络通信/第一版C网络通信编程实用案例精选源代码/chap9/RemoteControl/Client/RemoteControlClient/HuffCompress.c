#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "HuffCompress.h"
#include "Client.h"

DWORD	dwWeight[256];
DWORD	dwCounts[256];
DWORD	dwByte[256];
DWORD	dwCodes[512];
DWORD	dwSymbols[256];
DWORD	dwMapping[256];
DWORD	dwByteTree[765];
WORD	wByteTree[765];
char	dwBitTree[765];
DWORD	dwRootIndex,dwCodeCount,dwStorage;
WORD	wTreeSize;

int		iElements,iTotalElements;

void HuffmanInitArrays()
{
	__asm
	{
		MOV		EDI,OFFSET dwWeight[0]
		MOV		ECX,256
		_InitWeight:
		MOV		DWORD PTR [EDI],0
		ADD		EDI,4
		DEC		ECX
		JNZ		_InitWeight
	}

	__asm
	{
		MOV		EDI,OFFSET dwBitTree[0]
		MOV		EAX,0
		MOV		ECX,255
		_InitBitTree:
		MOV		char PTR [EDI + EAX],2
		ADD		EAX,3
		DEC		ECX
		JNZ		_InitBitTree
	}
}

void HuffmanBuildArrays()
{
	__asm
	{
		MOV		ESI,OFFSET dwWeight[0]
		MOV		EDI,OFFSET dwByte[0]

		MOV		ECX,256
		_BuildByte:

		LODSD
		CMP		EAX,0
		JZ		_SkipByte
		MOV		EAX,256
		SUB		EAX,ECX
		STOSD
		_SkipByte:
		DEC		ECX
		JNZ		_BuildByte
		MOV		ESI,OFFSET dwWeight[0]
		MOV		ECX,256
		MOV		EDI,OFFSET dwCounts[0]

		_BuildCount:

		LODSD

		CMP		EAX,0

		JZ		_SkipCount

		STOSD

		_SkipCount:
		DEC		ECX
		JNZ		_BuildCount

		MOV		ESI,OFFSET dwCounts[0]
		// Use the Pointers to get the Number of Elements
		SUB		EDI,ESI
		MOV		ECX,EDI
		SHR		ECX,2

		// Check for only 1 Element in the Byte Array
		CMP		ECX,1
		JNE		_Exit

		// Point to the Byte Array
		MOV		EDI,OFFSET dwByte[0]

		// Add the Dummy Byte (256 Can't Exist, so it is Fine)
		MOV		EAX,256
		MOV		[EDI + ECX * 4],EAX

		// Point to the Counts Array
		MOV		EDI,OFFSET dwCounts[0]

		// Add the Dummy Count of 1
		MOV		[EDI + ECX * 4],DWORD PTR 1

		// Increment the Number of Elements
		INC		ECX

		// Finished Building the Arrays
		_Exit:

		// Update the Number of Elements
		MOV		iElements,ECX
		MOV		iTotalElements,ECX
	}
}

// Build the Byte Tree
void HuffmanBuildByteTree()
{
	DWORD	dwParentWeight,dwParentDesc;
	int		iStartPos = -3;
	BOOL	fInsert = TRUE;

	// Arbitrary Value of the First Parent Combined Parent Description
	dwParentDesc = 1000;

	// While we don't have a Root Node
	while (iElements > 1)
	{
		// Quick Sort the Array
		HuffmanQuickSortD(&dwCounts[0],&dwByte[0],0,iElements - 1);

		// Build the Huffman Tree
		__asm
		{
			/* Step 1: Create a Parent for the Children with the
			           two lowest Weights and a Descriptor for
					   the Parent from the Children Bytes */

			// Point to the Count Array
			MOV		ESI,OFFSET dwCounts[0]

			// Compute the New End of Array
			MOV		EBX,iElements
			DEC		EBX

			// Update the Number of Elements
			MOV		iElements,EBX

			// Index the two Lowest Weights
			MOV		EAX,EBX
			DEC		EAX

			// Create a Parent from the Two Lowest Weights
			MOV		EDX,[ESI + EAX * 4]
			ADD		EDX,[ESI + EBX * 4]

			// Save the Parent
			MOV		dwParentWeight,EDX

			// Compute the Start Position in the Tree
			MOV		ECX,iStartPos
			ADD		ECX,3

			// Update the Start Position in the Tree
			MOV		iStartPos,ECX

			/* Step 3: Add to the Byte Conversion Tree */

			// Point to the Byte Array
			MOV		ESI,OFFSET dwByte[0]

			// Point to the Byte Table Conversion Tree
			MOV		EDI,OFFSET dwByteTree[0]

			// Move the New Parent Descriptor to the First Spot in Tree
			MOV		EDX,dwParentDesc
			MOV		[EDI + ECX * 4],EDX

			// Move the First Child Byte to the Second Spot in Tree
			MOV		EDX,[ESI + EAX * 4]
			MOV		[EDI + ECX * 4 + 4],EDX
			// Move the Second Child Byte to the Third Spot in Tree
			MOV		EDX,[ESI + EBX * 4]
			MOV		[EDI + ECX * 4 + 8],EDX

			/* Step 4: Update the Counts Array, Removing the Children
			           and Adding the Parent */

			// Point to the Count Array
			MOV		ESI,OFFSET dwCounts[0]

			// Restore the New Weight Value of the Parent
			MOV		EDX,dwParentWeight

			// Insert the Parent to the Counts Array
			MOV		[ESI + EAX * 4],EDX

			// Point to the Byte Array
			MOV		ESI,OFFSET dwByte[0]

			// Restore the New Weight Value of the Parent
			MOV		EDX,dwParentDesc

			// Insert the Combined Parent to the End of the Byte Array
			MOV		[ESI + EAX * 4],EDX

			// Update the Parent Descriptor for the Next New Parent
			INC		dwParentDesc
		}
	}

	// Get the Index to the Root in the Tree
	__asm
	{
		MOV		ECX,iStartPos
		MOV		dwRootIndex,ECX
		ADD		ECX,3
		MOV		wTreeSize,CX
	}
}

// Quick Sort the Array in Ascending Order
void HuffmanQuickSortA(DWORD *pArray1,DWORD *pArray2,int iBegin,int iEnd)
{
	int		iLow,iHigh,iTemp;

	__asm
	{
		// Set the Low and High Elements
		MOV		EAX,iBegin
		MOV		EBX,iEnd

		// Point the Input Array
		MOV		ESI,pArray1

		// Get the List Separator
		MOV		ECX,EAX
		ADD		ECX,EBX
		SHR		ECX,1
		MOV		EDX,[ESI + ECX * 4]

		// The Main Loop
		_DoLoop:

		// Point the Input Array
		MOV		ESI,pArray1

		// Order the Low Elements
		_OrderLow:
		MOV		EDI,EAX
		INC		EAX
		CMP		[ESI + EDI * 4],EDX
		JB		_OrderLow
		MOV		EAX,EDI

		// Order the High Elements
		_OrderHigh:
		MOV		EDI,EBX
		DEC		EBX
		CMP		[ESI + EDI * 4],EDX
		JA		_OrderHigh
		MOV		EBX,EDI

		// Check for Swapping Array[Low] with Array[High]
		CMP		EAX,EBX
		JG		_NoSwap

		// Swap dwCounts[Low] with dwCounts[High]
		MOV		ECX,[ESI + EAX * 4]
		MOV		EDI,[ESI + EBX * 4]
		MOV		[ESI + EAX * 4],EDI
		MOV		[ESI + EBX * 4],ECX

		// Swap the Byte Array
		MOV		ESI,pArray2
		MOV		ECX,[ESI + EAX * 4]
		MOV		EDI,[ESI + EBX * 4]
		MOV		[ESI + EAX * 4],EDI
		MOV		[ESI + EBX * 4],ECX

		// Update the Low and High Indexes
		INC		EAX
		DEC		EBX

		// Test for Looping Back
		_NoSwap:
		MOV		iTemp,EBX
		CMP		EAX,iTemp
		JLE		_DoLoop

		// Update the Low and High Values
		MOV		iLow,EAX
		MOV		iHigh,EBX
	}

	// Check for Sorting Elements between Begin and High (New Lower Range)
	if (iBegin < iHigh)
		HuffmanQuickSortA(pArray1,pArray2,iBegin,iHigh);

	// Check for Sorting Elements between Low and End (New Upper Range)
	if (iLow < iEnd)
		HuffmanQuickSortA(pArray1,pArray2,iLow,iEnd);
}

// Quick Sort the Array in Descending Order
void HuffmanQuickSortD(DWORD *pArray1,DWORD *pArray2,int iBegin,int iEnd)
{
	int		iLow,iHigh,iTemp;

	__asm
	{
		// Set the Low and High Elements
		MOV		EAX,iBegin
		MOV		EBX,iEnd

		// Point the Input Array
		MOV		ESI,pArray1

		// Get the List Separator
		MOV		ECX,EAX
		ADD		ECX,EBX
		SHR		ECX,1
		MOV		EDX,[ESI + ECX * 4]

		// The Main Loop
		_DoLoop:

		// Point the Input Array
		MOV		ESI,pArray1

		// Order the Low Elements
		_OrderLow:
		MOV		EDI,EAX
		INC		EAX
		CMP		[ESI + EDI * 4],EDX
		JA		_OrderLow
		MOV		EAX,EDI

		// Order the High Elements
		_OrderHigh:
		MOV		EDI,EBX
		DEC		EBX
		CMP		[ESI + EDI * 4],EDX
		JB		_OrderHigh
		MOV		EBX,EDI

		// Check for Swapping Array[Low] with Array[High]
		CMP		EAX,EBX
		JG		_NoSwap

		// Swap dwCounts[Low] with dwCounts[High]
		MOV		ECX,[ESI + EAX * 4]
		MOV		EDI,[ESI + EBX * 4]
		MOV		[ESI + EAX * 4],EDI
		MOV		[ESI + EBX * 4],ECX

		// Swap the Byte Array
		MOV		ESI,pArray2
		MOV		ECX,[ESI + EAX * 4]
		MOV		EDI,[ESI + EBX * 4]
		MOV		[ESI + EAX * 4],EDI
		MOV		[ESI + EBX * 4],ECX

		// Update the Low and High Indexes
		INC		EAX
		DEC		EBX

		// Test for Looping Back
		_NoSwap:
		MOV		iTemp,EBX
		CMP		EAX,iTemp
		JLE		_DoLoop

		// Update the Low and High Values
		MOV		iLow,EAX
		MOV		iHigh,EBX
	}

	// Check for Sorting Elements between Begin and High (New Lower Range)
	if (iBegin < iHigh)
		HuffmanQuickSortD(pArray1,pArray2,iBegin,iHigh);

	// Check for Sorting Elements between Low and End (New Upper Range)
	if (iLow < iEnd)
		HuffmanQuickSortD(pArray1,pArray2,iLow,iEnd);
}

// Navigate the Binary Tree
void HuffmanBuildCodes()
{
	DWORD	dwCodeBits;
	DWORD	dwLeafNode;

	__asm
	{
		// Initialize the Count of Codes
		MOV		dwCodeCount,0;

		// Initialize the Bit Code
		MOV		ECX,0

		// Initialize the Index
		MOV		EDX,dwRootIndex

		// Point to the Bit and Byte Tree Arrays
		MOV		ESI,OFFSET dwBitTree[0]
		MOV		EDI,OFFSET dwByteTree[0]

		// Initialize the Number of Bits in the Code
		MOV		dwCodeBits,0

		// While We Still Have Children for the Parent
		_WhileLoop:

		// Get the Number of Children
		MOV		EAX,0
		MOV		AL,[ESI + EDX]

		// Decrement the Parents Number of Children
		DEC		[ESI + EDX]

		// Check for Children
		CMP		EAX,0
		JE		_FindParent

		// Get the Last Child of the Current Parent
		ADD		EDX,EAX
		MOV		EBX,[EDI + EDX * 4]
		SUB		EDX,EAX

		// Increment the Code Bits
		ADD		ECX,ECX

		// For a Move to the Right in the Tree Add a 1 to the Code
		CMP		EAX,2
		JNE		_SkipOr
		OR		ECX,1
		_SkipOr:

		// Increment the Number of Bits in the Code
		INC		dwCodeBits

		// Check for a Leaf Node
		CMP		EBX,256
		JLE		_LeafNode

 		// Get the Parent Index of the Child Node Starting 
		// with the Previous Parent of the Current Parent
		_SubIndex:
		SUB		EDX,3
		CMP		EBX,[EDI + EDX * 4]
		JNE		_SubIndex

		// Loop Back for Another Node
		JMP		_WhileLoop

		// Process a Leaf Node
		_LeafNode:

		// Save the Index
		PUSH	EDX

		// Point to the Codes Array
		MOV		EAX,OFFSET dwCodes[0]

		// Set the Bit Count of the Symbol to the Code Array
		MOV		EDX,dwCodeBits
		MOV		[EAX + 1024 + EBX * 4],EDX

		// Set the Symbol to the Code Array
		MOV		[EAX + EBX * 4],ECX

		// Save the Leaf Node
		PUSH	ECX
		MOV		ECX,EBX
		MOV		dwLeafNode,ECX

		// Get the Current Index to the Codes Array
		MOV		EBX,dwCodeCount

		// Point to the Symbol Mapping Array
		MOV		EAX,OFFSET dwMapping[0]

		// Set the Mapping of the Symbol to the Code Array
		MOV		[EAX + EBX * 4],ECX

		// Restore the Leaf Node
		POP		ECX

		// Point to the Code Symbol Array
		MOV		EAX,OFFSET dwSymbols[0]

		// Set the Symbol to the Code Array
		MOV		[EAX + EBX * 4],ECX

		// Increment the Code Count
		INC		EBX
		MOV		dwCodeCount,EBX

		// Restore the Leaf Node
		MOV		EBX,dwLeafNode

		// Restore the Index
		POP		EDX

		// Decrement the Number of Bits in the Code
		DEC		dwCodeBits

		// Decrement the Bits
		SHR		ECX,1

		// Loop Back for Another Node
		JMP		_WhileLoop

		// Find the Parent Index of the Parent
		_FindParent:

		// Get the Current Parent
		MOV		EAX,dwRootIndex
		MOV		EBX,[EDI + EDX * 4]

		// Check the Children of the Parent for a Match
		_AddIndex:

		ADD		EDX,3
		// Check for Completion
		CMP		EDX,EAX
		JG		_Exit
		CMP		EBX,[EDI + (EDX + 4) * 4]
		JE		_SkipAdd
		CMP		EBX,[EDI + (EDX + 8) * 4]
		JNE		_AddIndex

		// Found the Index
		_SkipAdd:

		// Decrement the Number of Bits in the Code
		DEC		dwCodeBits

		// Decrement the Bits
		SHR		ECX,1

		// Go back for Another Node
		JMP		_WhileLoop
		_Exit:
	}
}

// Build the Byte Tree and Codes Table and Return the Tree Size
WORD HuffmanDictionary(BYTE *pInput,DWORD dwCount,DWORD *pByteTree,DWORD *pCodes)
{
	// Byte Tree Storage Requirement
	DWORD	dwStorage;

	// Initialize the Weights Array
	HuffmanInitArrays();

	// Sum the Occurences of the Data to the Weights Array
	__asm
	{
		// Point to the Source Data
		MOV		ESI,pInput

		// Point to the Destination Data
		MOV		EDI,OFFSET dwWeight[0]

		// Set the Loop Count
		MOV		ECX,dwCount
		MOV		EAX,0

		// Increment Each Occurence of the Byte
		_SumOccurences:
		LODSB

		// The Byte is an Index to the Weight Array
		INC		DWORD PTR [EDI + EAX * 4]
		DEC		ECX
		JNZ		_SumOccurences
	}

	// Build the Byte and Count Arrays
	HuffmanBuildArrays();

	// Build the Byte Tree
	HuffmanBuildByteTree();

	// Compute the Byte Tree Storage Requirements
	__asm
	{
		// Multiply the Tree Size by 4, for DWORD Access to the Byte Tree Array
		MOV		EDX,0
		MOV		DX,wTreeSize

		// Compute the Tree Storage Requirements
		SHL		EDX,2
		MOV		dwStorage,EDX
	}

	// Copy the Byte Tree
	memblast(pByteTree,&dwByteTree[0],dwStorage);

	// Build the Code List
	HuffmanBuildCodes();

	// Copy the Codes
	memblast(pCodes,&dwCodes[0],2048);

	// Return the Tree Size
	return wTreeSize;
}

// Compute the Compression Size of the Input Data
DWORD HuffmanCountCompress(BYTE *pInput,DWORD dwCount,WORD iTreeSize,DWORD *pCodes)
{
	// Variables Used for Code Shifting
	BOOL	fOutput = FALSE;

	// Compressed Size with Initial Space for Codes
	DWORD	dwNewCount;

	// Loop Variable
	DWORD	dwLoop;

	// Get the Tree Size and Table Storage Requirement
	__asm
	{
		// Get the Tree Size
		MOV		EAX,0
		MOV		AX,iTreeSize

		// Get the Table Storage
		SHL		EAX,2
		MOV		dwStorage,EAX

		// Add 6 for the DWORD and WORD that Stores the UnCompressed Count and Number of Tree Entries
		ADD		EAX,6

		// Initialize the Base Storage Requirement
		MOV		dwNewCount,EAX
	}

	// Encode the Input Data
	__asm
	{
		// Point to the Source Data
		MOV		ESI,pInput

		// Set the Loop Count
		MOV		EDX,dwCount
		MOV		dwLoop,EDX

		// Point to the Codes Array
		MOV		EDX,pCodes

		// Clear the Count of Used Bits
		MOV		EDI,0

		// Main Loop for Reading Bytes
		_LoopInput:

		// Clear the EAX Register for the Input Character
		MOV		EAX,0

		// Read the Input Byte and Increment the Input Pointer
		LODSB

		// Get the Compression Code (Bytes 0 - 255 in Array)
		MOV		EBX,[EDX + EAX * 4]

		// Get the Compression Code Bit Count (Bytes 256 - 511 in Array)
		MOV		ECX,[EDX + 1024 + EAX * 4]

		// See How Many Bits Can be Shifted to the Output DWORD
		ADD		EDI,ECX
		CMP		EDI,32
		JLE		_ShiftBits

		// See How Many Bits of the Last Code Can't Be Used
		SUB		EDI,32

		// Adjust ECX to Handle only the Bits that Can Be Used
		SUB		ECX,EDI

		// Set the Output Flag
		MOV		fOutput,TRUE

		// Start the Bit Shifting
		_ShiftBits:

		// Check for Outputting a full DWORD
		CMP		fOutput,FALSE
		JE		_Continue

		// Increment the Compression Count
		ADD		dwNewCount,4

		// Remove the Bits we Used
		MOV		EAX,32
		SUB		EAX,EDI
		MOV		ECX,EAX

		// Reset the Output Flag
		MOV		fOutput,FALSE

		// Decrement the Main Loop Counter
		_Continue:
		DEC		dwLoop
		JNZ		_LoopInput

		// Update Any Remaining Compression data in the Buffer
		CMP		EDI,0
		JE		_Exit

		// Left Shift the Remaining Bits
		MOV		EAX,32
		SUB		EAX,EDI
		MOV		ECX,EAX

		// Increment the Compression Count
		ADD		dwNewCount,4

		// Finished Compressing the Input Data
		_Exit:
	}

	// Return the Compression Count
	return dwNewCount;
}

// Compress the Count of Input Data to Output Data returning the new Count
DWORD HuffmanCompress(BYTE *pInput,DWORD dwCount,WORD iTreeSize,DWORD *pByteTree,DWORD *pCodes,BYTE *pOutput)
{
	// Allocation Variables
	BYTE	*pStartInput = pInput;
	BYTE	*pStartOutput = pOutput;

	// Variables Used for Code Shifting
	DWORD	dwUnusedByte = 0;
	BOOL	fOutput = FALSE;

	// Compressed Size with Initial Space for Codes
	DWORD	dwNewCount = 0;

	// Loop Variable
	DWORD	dwLoop;

	// Get the Tree Size and Table Storage Requirement
	__asm
	{
		// Get the Table Storage
		MOV		EAX,0
		MOV		AX,iTreeSize
		MOV		wTreeSize,AX
		SHL		EAX,2
		MOV		dwStorage,EAX
	}

	// Encode the Input Data
	__asm
	{
		// Point to the Destination Data
		MOV		EDI,pOutput

		// Store the UnCompressed Count
		MOV		EAX,dwCount
		MOV		[EDI],EAX

		// Move Past the UnCompressed Count
		ADD		EDI,4

		// Move in the Number of Codes to the Output
		MOV		ECX,0
		MOV		CX,wTreeSize
		MOV		[EDI],CX

		// Move Past the Number of Codes
		ADD		EDI,2

		// Initialize the Base Storage Requirement
		MOV		EDX,dwStorage
		MOV		dwNewCount,EDX

		// Add 6 for the DWORD and WORD that Stores the UnCompressed Count and Number of Tree Entries
		ADD		dwNewCount,6

		// Adjust the Start of the Output Compressed Data, Skipping Byte Tree
		SHR		EDX,1
		ADD		EDI,EDX

		// Store the Pointer to the Output
		MOV		pOutput,EDI

		// Set the Loop Count
		MOV		EDX,dwCount
		MOV		dwLoop,EDX

		// Point to the Codes Array
		MOV		EDX,pCodes

		// Clear the Count of Used Bits
		MOV		EDI,0

		// Initialize the Storage DWORD
		XOR		ESI,ESI

		// Main Loop for Reading Bytes
		_LoopInput:

		// Clear the EAX Register for the Input Character
		MOV		EAX,0

		// Save the ESI Register
		PUSH	ESI

		// Point to the Source Data
		MOV		ESI,pInput

		// Read an Input Byte
		MOV		AL,[ESI]

		// Increment the Pointer to the Source Data
		INC		ESI

		// Store the Pointer to the Source Data
		MOV		pInput,ESI

		// Restore the ESI Register
		POP		ESI

		// Get the Compression Code (Bytes 0 - 255 in Array)
		MOV		EBX,[EDX + EAX * 4]

		// Get the Compression Code Bit Count (Bytes 256 - 511 in Array)
		MOV		ECX,[EDX + 1024 + EAX * 4]

		// See How Many Bits Can be Shifted to the Output DWORD
		ADD		EDI,ECX
		CMP		EDI,32
		JLE		_ShiftBits

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

		// Start the Bit Shifting
		_ShiftBits:

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
		POP		EDI
		ADD		pOutput,4

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

		// Decrement the Main Loop Counter
		_Continue:
		DEC		dwLoop
		JNZ		_LoopInput

		// Update Any Remaining Compression data in the Buffer
		CMP		EDI,0
		JE		_Exit

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

		// Finished Compressing the Input Data
		_Exit:
	}

	// Set the Start of the Byte Tree
	pOutput = pStartOutput + 6;

	// Copy the DWORD Byte Tree to a WORD Byte Tree
	D2W(&wByteTree[0],pByteTree,dwStorage/4);

	// Copy the Byte Tree
	memblast(pOutput,&wByteTree[0],dwStorage/2);
	
	// Restore the Pointer to the Start of the Input
	pInput = pStartInput;

	// Restore the Pointer to the Start of the Output
	pOutput = pStartOutput;

	// Return the Compression Count
	return dwNewCount;
}

// Get the UnCompressed Count of Output Data
DWORD HuffmanGetSize(BYTE *pInput)
{
	DWORD	dwUnCount;

	__asm
	{
		// Point to the Source Data
		MOV		ESI,pInput

		// Get the UnCompressed Count
		MOV		EAX,[ESI]
		MOV		dwUnCount,EAX
	}

	// Return the UnCompressed Count
	return dwUnCount;
}

// UnCompress the Compressed Count of Input Data to Output Data returning the UnCompressed Count
DWORD HuffmanUnCompress(BYTE *pInput,BYTE *pOutput)
{
	// UnCompressed Count
	DWORD	dwUnCount;

	// Allocation Variables
	BYTE	*pStartInput = pInput;
	BYTE	*pStartOutput = pOutput;

	// UnCompressed Variables
	DWORD	dwLoop;

	// Initialize the Weights Array
	HuffmanInitArrays();

	// Copy the Decoding Dictionary
	__asm
	{
		// Point to the Source Data
		MOV		ESI,pInput

		// Get the UnCompressed Count
		MOV		EAX,[ESI]
		MOV		dwUnCount,EAX

		// Move Past the Uncompressed Count
		ADD		ESI,4

		// Get the Tree Size
		MOV		EAX,0
		MOV		AX,[ESI]
		MOV		wTreeSize,AX

		// Increment the Pointer to the Encoded Data
		ADD		ESI,2

		// Store the Start of the Compressed Data
		MOV		pInput,ESI

		// Compute the Root Index
		MOV		dwRootIndex,EAX
		SUB		dwRootIndex,3

		// Compute the Table Storage Requirement
		SHL		EAX,2
		MOV		dwStorage,EAX
	}

	// Copy the Decoding Byte Tree
	memblast(&wByteTree[0],pInput,dwStorage/2);

	// Convert the WORD Byte Tree to a DWORD Byte Tree
	W2D(&dwByteTree[0],&wByteTree[0],dwStorage/4);

	// Decode the Input Data
	__asm
	{
		// Increment the Pointer to the Data
		MOV		EAX,dwStorage
		SHR		EAX,1
		ADD		pInput,EAX

		// Point to the Byte Code Tree
		MOV		ESI,OFFSET dwByteTree[0]

		// Set the Loop Count for Amount of Bytes to Uncompress
		MOV		EDX,dwUnCount
		MOV		dwLoop,EDX

		// Calculate the Index of the Root
		MOV		EBX,dwRootIndex

		// Main Loop for Reading Bytes
		_LoopInput:

		// Point to the Source Data
		MOV		EDI,pInput

		// Read a DWORD of Encoded Data
		MOV		ECX,[EDI]

		// Increment the Source Data Pointer by a DWORD
		ADD		pInput,4

		// Initialize the Number of Bits to Process
		MOV		EAX,32

		// Left Shift the Encoded Data 1 Bit at a Time
		_LeftShift:

		// Initialize the Decode Storage
		MOV		EDX,0

		// Left Shift the Code into the Decode Storage
		SHL		ECX,1

		// Bit + 1 = Number of Children, ie The Direction in Tree to Move
		ADC		EDX,1

		// Index the Child of the Parent
		ADD		EBX,EDX

		// Decrement the Bits Processed
		DEC		EAX

		// Check for a Leaf Node
		MOV		EDI,[ESI + EBX * 4]
		CMP		EDI,256
		JLE		_LeafNode

		// Index the Child's Parent
		MOV		EDI,EBX
		SUB		EDI,EDX

		// Match the Child to a Parent
		_TestParent:

 		// Index the Previous Parent in the Tree
		SUB		EDI,3

		// Get the Child's Parent
		MOV		EDX,[ESI + EDI * 4]

		// Compare the Parent with the Child
		CMP		EDX,[ESI + EBX * 4]
		JNE		_TestParent

		// Update the Parent Index
		MOV		EBX,EDI

		// Check the Bits Processed upto this Point
		CMP		EAX,0

		// Check for More Bits to Process
		JNZ		_LeftShift

		// Read in another DWORD
		JMP		_LoopInput

		// Output a Decoded Symbol
		_LeafNode:

		// Point to the Destination Data
		MOV		EDI,pOutput

		// Output the Code
		MOV		EDX,[ESI + EBX * 4]
		MOV		[EDI],DL

		// Increment the Destination Data by 1 Byte
		INC		pOutput

		// Calculate the Index of the Root
		MOV		EBX,dwRootIndex

		// Decrement the Loop Counter
		DEC		dwLoop
		JZ		_Exit

		// Check for More Bits to Process
		CMP		EAX,0
		JNZ		_LeftShift

		// Read a New DWORD
		JMP		_LoopInput

		// Finished Decompressing
		_Exit:
	}

	// Restore the Pointer to the Start of the Input
	pInput = pStartInput;

	// Restore the Pointer to the Start of the Output
	pOutput = pStartOutput;

	// Return the Count of UnCompressed Data
	return dwUnCount;
}

// Copy a DWORD Array to a WORD Array
void D2W(void* dest,void* src,DWORD count)
{
	__asm
	{
		MOV		ESI,src		// Copy the Source Address to the Register
		MOV		EDI,dest	// Copy the Destination to the Register
		MOV		ECX,count	// Copy the Count to the Register

		_CopyD2W:
		MOV		EAX,[ESI]
		MOV		[EDI],AX
		ADD		ESI,4
		ADD		EDI,2
		DEC		ECX
		JNZ		_CopyD2W
	}
}

// Copy a WORD Array to a DWORD Array
void W2D(void* dest,void* src,DWORD count)
{
	__asm
	{
		MOV		ESI,src		// Copy the Source Address to the Register
		MOV		EDI,dest	// Copy the Destination to the Register
		MOV		ECX,count	// Copy the Count to the Register

		XOR		EAX,EAX
		_CopyW2D:
		MOV		AX,[ESI]
		MOV		[EDI],EAX
		ADD		ESI,2
		ADD		EDI,4
		DEC		ECX
		JNZ		_CopyW2D
	}
}