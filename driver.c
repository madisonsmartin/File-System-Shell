#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "filesystem.h"

//void printTokens(tokenlist* tokens){
//	node* cur = tokens->head;
//	while (cur != NULL)
//	{
//		printf("%s\n", cur->value);
//		cur = cur->next;
//	}
//}

int main(int argc, char* argv) {
	char input[1001];
	char lscwd[10];
	tokenlist* tokens;
	fsys* fs = newFileSystem();

	while(fgets(input, sizeof(input), stdin) != NULL) {
		tokens = tl_tokenize(input, " \n\r");
		char* command = tl_get(tokens, 0);

		// Empty line, end loop
		if(tokens->size == 0)
			break;

		if(strcmp(command, "mkdir") == 0) {
			fs_mkdir(fs, tl_get(tokens, 1));
		} else if(strcmp(command, "ls") == 0) {
			if(tokens->size == 1) {
				strcpy(lscwd, ".");
				fs_ls(fs, lscwd);
			} else
				fs_ls(fs, tl_get(tokens, 1));
		} else if(strcmp(command, "touch") == 0) {
			fs_touch(fs, tl_get(tokens, 1));
		} else if(strcmp(command, "cd") == 0) {
			if(tokens->size == 1) {
				fs_cd(fs, "/");
				//fs->cwd = fs->root;
			} else
				fs_cd(fs, tl_get(tokens, 1));
		} else if(strcmp(command, "pwd") == 0) {
			fs_pwd(fs);
		} else if(strcmp(command, "find") == 0) {
			fs_find(fs, tl_get(tokens, 1));
		} else if(strcmp(command, "rm") == 0) {
			if(tokens->size == 3) {
				fs_rmf(fs, tl_get(tokens, 2));
			} else
				fs_rm(fs, tl_get(tokens, 1));
		}

		// should delete these before submission, just testing cmdln
		else if(strcmp(command, "clear") == 0) {
			system("clear");
		} else if(strcmp(command, "exit") == 0) {
			exit(0);
		}
	}

	destroyFileSystem(fs);

	return 0;
}