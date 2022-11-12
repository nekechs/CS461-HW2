#ifndef _FILE_IO_
#define _FILE_IO_

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#ifndef _FDTYPES_
#define _FDTYPES_
#define FDTYPE_REG 0
#define FDTYPE_DIR 1
#define FDTYPE_OTHER 2
#endif

#ifndef OUTPUT_DIR
#define OUTPUT_DIR "."
#endif

typedef struct {
    char * data;
    size_t size;
} stream_t;

/**
 * @brief Takes in a file descriptor that we assume we can read from using read() and 
 * gives you a dynamic array of characters that stores all of the extracted information.
 * 
 * Note: MUST FREE THE POINTER AT SOME POINT AFTER USE 
 * 
 * @param fd - file descriptor from where you want to read the text from - ASSUMED TO BE READABLE
 * @return stream_t - stores pointer to a section in the heap that contains the raw dump from the file
 */
stream_t open_raw_data(int fd);

/**
 * @brief Function that is able to write a stream of data to a file. Takes in a file descriptor
 * and a stream_t and then writes the contents of stream_t to the file descriptor. Note: PLEASE
 * MAKE SURE THAT FILE YOU ARE WRITING TO IS VALID! Otherwise we may have significant issues pile up.
 * This function does NOT make sure that the file we are reading in is of type wrap.*, or that
 * the file descriptor is writable
 * 
 * Valid file descriptors are ones that are created with O_WRONLY
 * 
 * return value is used for checking for errors.
 *  returns -1: invalid file descriptor
 *  returns number of bytes written otherwise
 * 
 * @param fd - file descriptor we want to read in
 * @param stream - stores the data that we want to store (as well as data size)
 * @return int - 
 */
int write_stream_to_fd(int fd, stream_t stream);

/**
 * @brief Writes a string to stderr - mainly used for error reporting
 * 
 * @param string your error message - MUST BE NULL TERMINATED
 */
void write_stderr(char * string);

/**
 * @brief Gets the type of the file. Returns 0 for REG descriptor, 1 for DIR descriptor,
 * and 2 for some other format of descriptor.
 * 
 * If fstat returns an error, then this function returns -1.
 * 
 * For easier use, see macros FDTYPE_DIR and FDTYPE_REG
 * 
 * @param fd - file descriptor to be analyzed
 * @return int - type of descriptor
 */
int getfdtype(int fd);

#endif