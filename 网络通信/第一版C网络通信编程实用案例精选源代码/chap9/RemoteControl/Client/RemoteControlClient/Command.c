#include "Command.h"
#include "client.h"
#include <windows.h>

struct CommandList *Add_Command(struct CommandList *pNode,struct CommandDS Command)
{
	if (pNode->pNext = (struct CommandList *)malloc(sizeof(struct CommandList)))
	{
		pNode = pNode->pNext;
		strcpy(pNode->Command.szElement,Command.szElement);
		pNode->pNext = NULL;
		return pNode;
	}
	return NULL;
}

void Clear_Command(struct CommandList *pStart)
{
	struct	CommandList	*pPrev;
	struct	CommandList	*pNode;
	while (pNode = pStart->pNext)
	{
		pPrev = pStart;
		pPrev->pNext = pNode->pNext;
		free(pNode);
	}
}