#include "tokenlist.h"

void tl_append(tokenlist* list, char* value){
	node* newNode = (node*)malloc(sizeof(node));
	newNode->value = value;
	newNode->next = NULL;

	if (list->head == NULL){
		list->head = newNode;
	}
	else{
		node* cur = list->head;
		while (cur->next != NULL)
			cur = cur->next;
		cur->next = newNode;
	}

	list->size++;
}

char* tl_get(tokenlist* list, int index){
	node* cur = list->head;
	int i = 0;

	if (index < 0 || index >= list->size)
		return NULL;

	while (cur != NULL && i < index){
		cur = cur->next;
		i++;
	}

	return cur->value;
}

tokenlist* tl_newList(){
	tokenlist* result = (tokenlist*)malloc(sizeof(tokenlist));
	result->head = NULL;
	result->size = 0;
}
