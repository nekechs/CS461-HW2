#ifndef _STRINGAPPEND_H_
#define _STRINGAPPEND_H_

/**
 * @brief Returns a pointer to a null terminated string (residing on the heap)
 * that is essentially a path to the file based on the specified directory.
 * 
 * Example:
 * append_to_dir("Documents", "foo") returns "Documents/foo"
 * append_to_dir("Documents/", "foo") returns "Documents/foo"
 * 
 * Note: the pointer returned is MALLOC'D, which means it MUST be freed after
 * calling this function. It is up to the callee to do that.
 * 
 * @param dir - string to directory name; MUST BE NULL TERMINATED
 * @param filename - string to file name; MUST BE NULL TERMINATED
 * @return char* - New string that is dir/filename
 */
char * append_to_dir(char * dir, char * filename);

/**
 * @brief Returns a pointer to a null terminated string (on heap) that is
 * basically the first string appended to the second string.
 * 
 * Example:
 * append_to_string("bruh", "moment") returns "bruhmoment"
 * append_to_string("wrap.", "file") returns "wrap.file"
 * 
 * @param str1 
 * @param str2 
 * @return char* 
 */
char * append_to_string(char * str1, char * str2);

/**
 * @brief Given some heap allocated string, we can reallocate that memory, and have the pointer
 * to the string point to some new string, where the string contains the original string with
 * some other string concatenated to it. 
 * 
 * @param base Points to some string reference in memory.
 * @param addition Some strint to be added to the base string.
 */
void append_after(char ** base, char * addition);

/**
 * @brief Returns a pointer to a null terminated string (on heap) that is
 * basically the input string append with ".wrap" extention 
 *
 * Example:
 * append_wrap_prefix("bruh") returns "wrap.bruh"
 * append_wrap_prefix("test/bruh") returns "test/wrap.bruh"
 *
 * @param str1
 * @return char*
 *
 * NOTE: the caller is responsible for free up the memory of the returned char* 
 */
char * append_wrap_prefix(char * str1);

/**
 * @brief Generates a dynamically allocated string that represents the path to a
 * new file that starts with wrap., but it also includes the path to the file itself.
 * 
 * MAKE SURE TO FREE THE GENERATED PATH AFTER USING IT
 * 
 * @param path path to original file
 * @return char* malloc'd string that contians the path to the file you want
 */
char * gen_wrap_path(char * path);



#endif
