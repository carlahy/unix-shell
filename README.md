# unix-shell
COMP3005: Operating Systems
The aim of this coursework is to write a simplified Unix command interpreter in C, similar to bash or csh in purpose.

This directory contains the files:

- Shell.c
  - Contains the main function that makes high level calls to the other source files that handle the shell functionalities.

- Parse.c
  - Contains functions to parse the directories and the user input, identify commands and identify what directory the command is in. The directories are parsed based on the PATH profile variable. The user input is parsed as a command followed by one or multiple parameters.

- Directories.c: 
  - Contains the logic for a singly linked list that stores the directories so they are easily iterable. Function to find what directory a command belongs to, and vice versa whether a command belongs to a directory. 

- Execute.c: 
  - Contains the functions to handle environment variables (reading from profile file, reassigning HOME and PATH) and the functionalities relating to the shell and its processes (prompting user for input, forking a process, changing directories in parent process).
