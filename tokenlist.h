#ifndef _TOKENLIST_
#define _TOKENLIST_

#include <stdlib.h>

typedef struct _token_node node;
struct _token_node{
	char * value;
	node * next;
};

typedef struct _tokenlist
{
	node * head;
	int size;
}tokenlist;

void tl_append(tokenlist* list, char* value);
char* tl_get(tokenlist* list, int index);
tokenlist* tl_newList();

#endif