#if !defined(COMMAND_H)
#define COMMAND_H

#include <windows.h>

struct CommandDS
{
	char	szElement[81];
};

struct CommandList
{
	struct CommandDS	Command;
	struct CommandList	*pNext;
};

struct CommandList	*Add_Command(struct CommandList *pNode,struct CommandDS Command);
void		Clear_Command(struct CommandList *pStart);

#endif 