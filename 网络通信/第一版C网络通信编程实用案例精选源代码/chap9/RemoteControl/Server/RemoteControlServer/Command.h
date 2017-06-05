#if !defined(COMMAND_H)
#define COMMAND_H

#include <windows.h>

// 命令栈数据结构
struct CommandDS
{
	char	szElement[81];
};

// 命令栈链表
struct CommandList
{
	struct	CommandDS	Command;
	struct	CommandList	*pNext;
};

struct	CommandList	*Add_Command(struct CommandList *pNode,struct CommandDS Command);
void				Clear_Command(struct CommandList *pStart);

#endif 