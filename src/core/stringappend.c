#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "fileio.h"
#include "stringappend.h"

// Make a macro for path seperator
#define PATH_SP '/'
// a b c d e f \0
// a b c d e f / \0
char * append_to_dir(char * dir, char * filename) {
    // # characters in directory name
    int dirsize = strlen(dir);
    dirsize = dir[dirsize - 1] == '/' ? dirsize - 1 : dirsize;

    // # characters in filename
    int filesize = strlen(filename);

    // Allocate space for new string
    char * new_string = malloc(dirsize + filesize + 2);

    if(new_string == NULL) {
        perror("Malloc failed when trying to append filename to directory");
        exit(EXIT_FAILURE);
    }

    // Copy over the directory name to new_string
    memcpy(new_string, dir, dirsize);

    // Add the path seperator just after directory name
    new_string[dirsize] = PATH_SP;

    // Copy the file name to rest of new_string, INCLUDING the null terminator
    memcpy(new_string + dirsize + 1, filename, filesize + 1);

    return new_string;
}

char * append_to_string(char * str1, char * str2) {
    int first_size = strlen(str1);
    int second_size = strlen(str2);
    char * new_string = malloc(first_size + second_size + 1);

    if(new_string == NULL) {
        write_stderr("Malloc failed when trying to append string to directory");
        exit(EXIT_FAILURE);
    }

    // new_string[0] = '\0';   // This should be empty null terminated or strcat will fail

    // strcat(new_string, str1);
    // strcat(new_string, str2);

    memcpy(new_string, str1, first_size);
    memcpy(new_string + first_size, str2, second_size);

    new_string[first_size + second_size] = '\0';    

    return new_string;
}

void append_after(char ** base, char * addition) {
    char * tmp = *base;
    *base = append_to_string(*base, addition);
    free(tmp);
}

char* append_wrap_prefix(char* filepath) {

	char * filename;
	int pos = 0;
	char * slash = strchr(filepath, '/');
	//char * posStr= filepath;
	
	while (slash) {
		printf("============ find slash %s\n", slash);
	        pos = strlen(filepath) - strlen(slash) +1;	
		//printf("   --------pos = %d\n", pos);

		slash = strchr(filepath+pos, '/');
	} 

	int len = strlen(filepath) - pos;
	char * name = malloc(len+1);
	//posStr = posStr+pos;
	//strcpy(name, posStr);
	strcpy(name, filepath+pos);
	char * wrapname = append_to_string("wrap.", name);

	// attach name to the directory path
	char* dir = malloc(pos+1);
	memset(dir, 0, pos+1);
        strncpy(dir, filepath, pos);
	//printf("   --------pos = %d len=%d  name=%s wrapname=%s dir=%s\n",  pos, len,  name, wrapname, dir);

	filename = append_to_string(dir, wrapname);	
        //printf("  wrapname = %s dir =%s filename=%s\n", wrapname, dir, filename);
	free(wrapname);
	free(name);
	free(dir);

	return filename;
}


char * gen_wrap_path(char * path) {
    int orig_length = strlen(path);

    if(orig_length == 0) return NULL;

    // Account for the last character being a path separator
    orig_length = path[orig_length - 1] == '/' ? orig_length - 1 : orig_length;

    // We add 6 because of null terminator and 5 chars in "wrap."
    char * wrap_path = malloc(orig_length + 6);
    
    // Use this index to keep track of the index of the last path separator (bef file name)
    int path_endind;
    for(path_endind = orig_length - 1; path_endind >= 0; path_endind--) {
        if(path[path_endind] == '/') {
            path_endind--;
            break;
        }
    }

    // If path seperator index is -1, then assume that the path is the filename itself.
    if(path_endind > -1 || path[0] == '/') {
        memcpy(wrap_path, path, path_endind + 1);
        wrap_path[path_endind + 1] = '/';

        memcpy(wrap_path + path_endind + 2, "wrap.", 5);
        memcpy(wrap_path + path_endind + 7, path + path_endind + 2, orig_length - path_endind - 2);
        wrap_path[orig_length + 5] = 0;
    } else {
        memcpy(wrap_path, "wrap.", 5);
        memcpy(wrap_path + 5, path, orig_length);
        wrap_path[orig_length + 5] = '\0';
    }


    // epic
    // wrap.pic

    // 0 1 2 3 4 5 6 7 8 9 1011121314
    // b r u h / e p i c \0
    // b r u h / w r a p . e p i c \0
    
    return wrap_path;
 }
