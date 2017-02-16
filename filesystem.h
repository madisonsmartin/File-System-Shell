#ifndef ___fs_H_
#define ___fs_H_

#include <stdlib.h>
#include <stdio.h>
#include "tokenlist.h"

#define MAX_LNAME_LENGTH 21
#define MAX_PATH_LENGTH 1001

typedef struct _file_system_node FSNode;
struct _file_system_node{
	char name[21];
	char id;
	FSNode * frstChld;
	FSNode * nxtSblng;
	FSNode * parnt;
};

typedef struct _file_system fsys;
struct _file_system{
	FSNode * root;
	FSNode * cwd;
};

//file system funtions
fsys* newFileSystem();
void destroyFileSystem(fsys*);
void fs_ls(fsys*, char*);
void fs_pwd(fsys*);
void fs_mkdir(fsys*, char*);
void fs_touch(fsys*, char*);
void fs_cd(fsys*, char*);
void fs_rm(fsys*, char*);
void fs_rmf(fsys*, char*);
void fs_find(fsys*, char*);

FSNode* newFsNode(char* name, char type, FSNode* parent);
tokenlist* tl_tokenize(char *, char *);

// Temp
char * getLocalName(char * p, char * ln);
void printAbsPath(FSNode* node);
void update(char * orig, char* copy);
int compareStr(char * a, char * b);
int compareNode(FSNode * a, FSNode * b);
void recFind(FSNode * node, char * name);
FSNode * follow(FSNode * fnode, tokenlist * tokens, int pos);
void insertChild(FSNode * parent, FSNode * child);
FSNode*  insertRest(FSNode * start, tokenlist * tokens);
void printNode(FSNode* node);
#endif