#include "Command.h"
#include "Server.h"
#include <windows.h>

// 添加一个元素到命令栈中
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

//完全清除命令栈元素
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