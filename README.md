# File-System-Shell
Class project where we had to implement a primitive file-system shell. 

BASIC PROJECT SPECS

For this assignment you will be implementing a primitive file-system shell. Your
program will need to be able to navigate paths and traverse a directory structure.
For the directory structure you will want to create an n-ary tree where the
number of children is not limited. There are two ways to achieve this. One way is
to change nodes from having 2 fixed children to having a list of children. Another
way is to change the meaning from `left' and `right' pointers to `nextsibling' and
`firstchild' pointers. This last pattern simulates an n-ary tree through a binary
tree. A parent node only points to the first child out of its children and has to
walk the nextsibling pointer from the first child to get to the second child, walk
the nextsibling pointer from the second child to get to the third child, and so
on.

Commands given from commandline:
  ls
  ls <path>
  pwd
  mkdir <path>
  touch <path>
  cd <path>
  rm <path>
  rm -f <path>
  find <local name>
  
Core Concepts:
  1. Path - navigation of the directory structures
  2. cwd - Current Working Directory (for executing commands)
  3. pwd - Path from root directory to cwd
  4. ls - list contents of cwd or specified directory.
  5. mkdir - create a directory at the specified path.
  6. touch - create a file in the proper directory
  7. cd - change the cwd to the directory specified by the path.
  8. rm - remove specified file or directory (error if directory not empty)
  9. rm -f - force remove of specified directory, deleting all sub-directories and their content.
  10. find - list the paths to all directories and files with the a matching local-name.
  
ORDER OF CHILDREN IN DIRECTORY
  The contents of a directory are saved on a list in the following order:
    1. Directories are listed before files.
    2. Within directories and files regions, contents are sorted in the follow-ing lexicographic ordering (semi dictionary order)
        { '.' < '-' < ' ' < (0-9) < (A-Z) < (a-z)
  You may need to use a custom compare function to compare character by character to guide your inserting new files and directories in proper order.
