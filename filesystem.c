#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include "filesystem.h"

FSNode * follow(FSNode * fnode, tokenlist * tokens, int pos);
FSNode * findChild(FSNode * node, char * name);
void printNode(FSNode* node);
void insertChild(FSNode * parent, FSNode * child);
FSNode*  insertRest(FSNode * start, tokenlist * tokens);
void recRemove(FSNode * fnode);
void removeNode(FSNode* parent, FSNode* node);
FSNode* getLastExisting(FSNode* start, tokenlist* tokens, int* lastindex);


void destroyFileSystem(fsys * fs) {
	while(fs->root->frstChld != NULL) {
		recRemove(fs->root->frstChld);
	}

	free(fs->root);
	free(fs);
}

void fs_ls(fsys * fs, char * p) {
	char path[1001];
	strcpy(path, p);

	FSNode* start = p[0] == '/' ? fs->root : fs->cwd; //condition ? a : b;
	tokenlist * tokens = tl_tokenize(p, "/");

	FSNode * fnode = follow(start, tokens, 0);

	if(fnode == NULL) {
		int lastindex = -1;
		FSNode* last = getLastExisting(start, tokens, &lastindex);
		if (last == NULL){
			printf("Path Error: directory '/%s' does not exist.\n", tl_get(tokens, lastindex + 1));
		}
		else{
			printf("Path Error: directory '");
			printAbsPath(last);
			printf("/%s' does not exist.\n", tl_get(tokens, lastindex + 1));
		}
		printf("List Error: Cannot perform list operation.\n");
	} else if(fnode->id == 'F') {
		printf("List Error: Cannot perform list operation '");
		printAbsPath(start);
		if(path[0] != '/')
			printf("/");
		if(path)
			printf("%s' is a file\n", path);
	} else {
		printf("Listing For '");
		printAbsPath(fnode);
		if(path[0] != '/' && fnode == fs->root)
			printf("/");
		printf("':\n");

		FSNode * cur = fnode->frstChld;
		while(cur != NULL) {
			printNode(cur);
			cur = cur->nxtSblng;
		}
	}
}

//TODO: Follow Error Cae:
//Path Error: directory 'absPath to missing dir' does not exsit.
void fs_mkdir(fsys * fs, char * p) {
	// copy p into path to use original string to use in function
	char path[1001];
	strcpy(path, p);

	FSNode* start = p[0] == '/' ? fs->root : fs->cwd; //condition ? a : b;
	int safe = 0;
	tokenlist * tokens = tl_tokenize(p, "/");
	int size = tokens->size;
	int i;

	// Make sure that all previous nodes are directories (or don't exist)
	FSNode* fnode;
	for(i = 0; i < size; i++) {
		tokens->size = i + 1;
		fnode = follow(start, tokens, 0); //need access outside of loop
		if(fnode == NULL) {
			safe = 1;
			break;
		}

		else if (fnode->id == 'F' && i < (size - 1)){
			printf("Path Error: Cannot create sub-directory content. '");
			printAbsPath(fnode); //print abs path to file
			printf("' is a file.\n");
			break;
		}
	}

	if(safe == 0) {
		if(i == size) {
			printf("Path Error: '");
			printAbsPath(fnode);
			printf("' already exists and cannot be created.\n");
		}
		printf("Make Dir Error: Cannot create directory.\n");
		return;
	}
	if(i > 0) {  //items from 0 to i-1 exsit
		tokens->size = i;
		FSNode * cur = follow(start, tokens, 0); //geting last existing node
		//adjust token list to non-existing nodes
		int j;
		for(j = 0; j < i; j++) {
			tokens->head = tokens->head->next;
		}
		tokens->size = size - i;
		//insert non-existing nodes
		insertRest(cur, tokens);
	} else {  //nothing curently existed
		tokens->size = size;
		insertRest(start, tokens);
	}
}

void fs_pwd(fsys * fs) {
	printAbsPath(fs->cwd);
	printf("%s\n", fs->cwd == fs->root ? "/" : "");
}

void fs_touch(fsys * fs, char * p) {
	char path[1001];
	strcpy(path, p);

	FSNode* start = p[0] == '/' ? fs->root : fs->cwd; //condition ? a : b;
	int safe = 0;
	tokenlist * tokens = tl_tokenize(p, "/");
	int size = tokens->size;
	int i;

	// Make sure that all previous nodes are directories (or don't exist)
	FSNode* fnode;
	for(i = 0; i < size; i++) {
		tokens->size = i + 1;
		fnode = follow(start, tokens, 0);
		if(fnode == NULL) {
			safe = 1;
			break;
		}
		//when a/file exsits, touch a/file is called, this prints and it shouldn't
		//following block breaks the loop so i != size and correct error msg doesn't print
		else if (fnode->id == 'F' && i < (size-1)){
			printf("Path Error: Cannot create sub-directory content. '");
			printAbsPath(fnode); //print abs path to file
			printf("' is a file.\n");
			break;
		}
	}
	if(safe == 0) {
		if (i == size) {
			printf("Path Error: '");
			printAbsPath(fnode);
			printf("' already exists and cannot be created.\n");
		}
		printf("Touch Error: Cannot create file.\n");
		return;
	}
	tokens->size = i;
	FSNode * cur = follow(start, tokens, 0); //geting last existing node
	//adjust token list to non-existing nodes
	int j;
	for(j = 0; j < i; j++) {
		tokens->head = tokens->head->next;
	}
	tokens->size = size - i;

	char* fileName = tl_get(tokens, tokens->size - 1);
	tokens->size = tokens->size - 1;

	//Insert previous directories
	FSNode* last = insertRest(cur, tokens);
	FSNode * newNode = newFsNode(fileName, 'F', last);
	insertChild(last, newNode);
}

void fs_cd(fsys * fs, char * p) {
	FSNode* start = p[0] == '/' ? fs->root : fs->cwd; //condition ? a : b;
	tokenlist * tokens = tl_tokenize(p, "/");
	FSNode * fnode = follow(start, tokens, 0);
	if (fnode == NULL){
		int lastindex = -1;
		FSNode* last = getLastExisting(start, tokens, &lastindex);
		if (last == NULL){
			printf("Path Error: directory '/%s' does not exist.\n", tl_get(tokens, lastindex + 1));
		}
		else{
			printf("Path Error: directory '");
			printAbsPath(last);
			printf("/%s' does not exist.\n", tl_get(tokens, lastindex + 1));
		}
		printf("Change Dir Error: Cannot change working directory.\n");
	}
	else if (fnode->id == 'F')
		printf("Change Dir Error: Cannot change working directory.\n");
	else {
		fs->cwd = fnode;
	}
}

void recRemove(FSNode * fnode) {
	if(fnode == NULL) {
		return;
	} else if(fnode->id == 'F' || (fnode->id == 'D' && fnode->frstChld == NULL)) {
		// Leaf node or empty directory
		removeNode(fnode->parnt, fnode);
		return;
	}

	while(fnode->frstChld != NULL) {
		recRemove(fnode->frstChld);
	}

	removeNode(fnode->parnt, fnode);
}

void fs_rmf(fsys * fs, char * p) {
	FSNode* start = p[0] == '/' ? fs->root : fs->cwd; //condition ? a : b;
	tokenlist * tokens = tl_tokenize(p, "/");

	FSNode * fnode = follow(start, tokens, 0);
	if(fnode == NULL || fnode == fs->root) {
		int lastindex = -1;
		FSNode* last = getLastExisting(start, tokens, &lastindex);
		if (last == NULL){
			printf("Path Error: directory '/%s' does not exist.\n", tl_get(tokens, lastindex + 1));
		}
		else{
			printf("Path Error: directory '");
			printAbsPath(last);
			printf("/%s' does not exist.\n", tl_get(tokens, lastindex + 1));
		}
		printf("Remove Error: Cannot remove file or directory.\n");
		return;
	}

	// Recursively remove node's children and then free it
	recRemove(fnode);
}

void fs_rm(fsys * fs, char * p) {
	FSNode* start = p[0] == '/' ? fs->root : fs->cwd; //condition ? a : b;
	tokenlist * tokens = tl_tokenize(p, "/");
	FSNode * fnode = follow(start, tokens, 0);

	if(fnode == NULL || fnode == fs->root) {
		int lastindex = -1;
		FSNode* last = getLastExisting(start, tokens, &lastindex);
		if (last == NULL){
			printf("Path Error: directory '/%s' does not exist.\n", tl_get(tokens, lastindex + 1));
		}
		else{
			printf("Path Error: directory '");
			printAbsPath(last);
			printf("/%s' does not exist.\n", tl_get(tokens, lastindex + 1));
		}
		printf("Remove Error: Cannot remove file or directory.\n");
		return;
	}

	// Cannot delete non-empty directory
	if(fnode->id == 'D' && fnode->frstChld != NULL) {
		printf("Remove Error: directory '");
		printAbsPath(fnode);
		printf("' is not empty.\n");
		return;
	}

	// Remove and free the node
	removeNode(fnode->parnt, fnode);
}

void fs_find(fsys * f, char * p) {
	recFind(f->root, p);
}

//Returns true when b is a substring of a and false otherwise
int subStr(char * a, char * b) {
	char acopy[21];
	char bcopy[21];
	update(a, acopy);
	update(b, bcopy);
	if(strstr(acopy, bcopy) == NULL)
		return 0;
	return 1;
}

//The helper function for find
void recFind(FSNode * node, char * name) {
	if(node == NULL)
		return;

	// We've found one with that name...
	if(subStr(node->name, name)) {
		printf("%c ", node->id);
		printAbsPath(node);
		printf("\n");
	}

	// Recursively search through children
	FSNode * child = node->frstChld;
	while(child != NULL) {
		recFind(child, name);
		child = child->nxtSblng;
	}
}

FSNode* newFsNode(char* name, char type, FSNode* parent) {
	FSNode* result = (FSNode*)malloc(sizeof(FSNode));
	result->id = type;
	result->parnt = parent;
	strcpy(result->name, name);
	result->frstChld = result->nxtSblng = NULL;

	return result;
}

fsys* newFileSystem() {
	fsys* result = (fsys*)malloc(sizeof(fsys));
	result->root = newFsNode("root", 'D', NULL);
	result->cwd = result->root;

	return result;
}

FSNode * findChild(FSNode * node, char * name) {
	FSNode * child = node->frstChld;
	while(child != NULL) {
		if(compareStr(child->name, name) == 0)
			return child;
		child = child->nxtSblng;
	}
	return NULL;
}

//Returns a node at the end of the path given
FSNode * follow(FSNode * fnode, tokenlist * tokens, int pos) {
	if(pos >= tokens->size) {
		return fnode;
	}

	char * token = tl_get(tokens, pos);

	if(strcmp(token, ".") == 0) {//stay
		if((pos + 1) >= tokens->size) {
			return fnode;
		} else {
			return follow(fnode, tokens, pos + 1);
		}
	} else if(strcmp(token, "..") == 0) { //go to parent
		if (fnode->parnt == NULL) //root node
			return follow(fnode, tokens, pos + 1); //stay
		else if((pos + 1) >= tokens->size)
			return fnode->parnt;
		else
			return follow(fnode->parnt, tokens, pos + 1); //go to parent			
	} else {
		//find child
		FSNode * child = findChild(fnode, token);
		if(child == NULL) {
			return NULL;  //not there, invalid path
		}
		//was there
		if((pos + 1) >= tokens->size) {//no more tokens
			return child;
		} else {
			return follow(child, tokens, pos + 1);
		}
	}
}

//Prints absolute path to a node
void printAbsPath(FSNode* node) {
	if(node->parnt == NULL) return;
	printAbsPath(node->parnt);
	printf("/%s", node->name);
}

//Creates a copy of the original string that obeys the lexicographical order 
void update(char * orig, char* copy) {
	int i = strlen(orig);
	if(i == 0)
		copy[0] = '\0';
	else {
		strcpy(copy, orig);
		int j;
		for(j = 0; j < i; j++) {
			if(copy[j] == '.')
				copy[j] = 32;
			else if(copy[j] == '-')
				copy[j] = 33;
			else if(copy[j] == '_')
				copy[j] = 34;
		}
	}
}

//Compared 2 strings based on given order
int compareStr(char * a, char * b) {
	char acopy[21];
	char bcopy[21];
	update(a, acopy);
	update(b, bcopy);
	return strcmp(acopy, bcopy);
}

//Compares 2 nodes based on given order
int compareNode(FSNode * a, FSNode * b) {
	if(a->id == b->id)
		return compareStr(a->name, b->name);
	else if(a->id == 'D')
		return -1;
	else
		return 1;
}

// Given a path, returns a list of tokens separated by characters in delim
tokenlist * tl_tokenize(char * path, char * delim) {
	char * token;
	tokenlist* tokens = tl_newList();

	token = strtok(path, delim);
	while(token != NULL) {
		tl_append(tokens, token);
		token = strtok(NULL, delim);
	}

	return tokens;
}

void printNode(FSNode* node) {
	printf("%c %s\n", node->id, node->name);
}

void insertChild(FSNode * parent, FSNode * child) {
	if(parent->frstChld == NULL) {
		parent->frstChld = child;
		return;
	}
	FSNode * prev = NULL;
	FSNode * cur = parent->frstChld;

	while(cur != NULL && compareNode(child, cur) > 0) {
		prev = cur;
		cur = cur->nxtSblng;
	}
	if(prev == NULL) {  // Should be first
		child->nxtSblng = cur;
		parent->frstChld = child;
	} else if(cur == NULL) {  // Should be last
		prev->nxtSblng = child;
	} else {  // Between two nodes
		prev->nxtSblng = child;
		child->nxtSblng = cur;
	}
}

// Inserts all tokens in the list as directories and subdirectories
// and returns the last directory inserted
FSNode* insertRest(FSNode * start, tokenlist * tokens) {
	int i;
	FSNode * cur = start;
	for(i = 0; i < tokens->size; i++) {
		char* token = tl_get(tokens, i);

		FSNode * newNode = newFsNode(token, 'D', cur);
		insertChild(cur, newNode);
		cur = newNode;
	}

	return cur;
}

// Removes the given node from it's parent
void removeNode(FSNode* parent, FSNode* node) {
	// Head of list, update parent's pointer and delete
	if(node == parent->frstChld) {
		parent->frstChld = node->nxtSblng;
	} else {
		// Not head, fix chain of pointers
		FSNode* cur = parent->frstChld;
		while(cur->nxtSblng != node) {
			cur = cur->nxtSblng;
		}

		cur->nxtSblng = node->nxtSblng;
	}

	free(node);
}

// Returns last existing node when rest of path doe not exist
FSNode* getLastExisting(FSNode* start, tokenlist* tokens, int* lastindex){
	int size = tokens->size;
	FSNode* last = NULL;
	FSNode* fnode;
	*lastindex = -1;
	int i = 0;
	for (i = 0; i < size; i++){
		tokens->size = i + 1;
		fnode = follow(start, tokens, 0);
		if (fnode == NULL){
			tokens->size = size;
			return last;
		}
		*lastindex = i;
		last = fnode;
	}
	//should never reach this
	tokens->size = size;
	*lastindex = -1;
	return NULL;
}