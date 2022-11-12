#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "fileio.h"
#include "linkedlist.h"

#define BUFSIZE 4096

#ifndef DEBUG
#define DEBUG 0
#endif

stream_t open_raw_data(int fd) {
    // You cannot feed in an invalid file descriptor
    if(fd < 0) {
        printf("s\n");
        perror("Fed an invalid file descriptor into open_raw_data.");
        
        stream_t stream;
        stream.data = NULL;
        stream.size = 0;
        return stream;
    }
    stream_t stream;

    // Buffer we use with read()
    char buffer[BUFSIZE];

    // declare and initialize the linked list
    linkedlist_t orig_list;
    linkedlist_t * data_list  = &orig_list;
    ll_init(data_list, BUFSIZE);

    if(DEBUG) puts("Initialized linked list");
    
    node_t * last_node = data_list->head;
    
    // We use bytes to keep track of currently read no. of bytes and we use
    // total_data_size to keep track of the size of the data read.
    int bytes;
    size_t total_data_size = 0;

    while((bytes = read(fd, buffer, BUFSIZE)) > 0) {
        last_node = ll_insert_after_node(data_list, last_node, buffer, bytes);
        total_data_size += bytes;
    }

    if(DEBUG) puts("FIlled up linked list");

    // Allocate a segment in memory equal to no. of chars read
    stream.data = malloc(total_data_size);
    stream.size = total_data_size;

    // For the elements of the linked list, dump the contents of the elements
    // sequentially into our stream
    size_t bytes_left = total_data_size;
    while(data_list->head != NULL) {
        memcpy(stream.data + (total_data_size - bytes_left), 
            data_list->head->node_data, data_list->head->node_occupied);

        bytes_left -= data_list->head->node_occupied;
        ll_delete_front(data_list);
    }

    // we do this to free the memory used by list
    ll_destroy(data_list);

    return stream;
}

int write_stream_to_fd(int fd, stream_t stream) {
    if(fd == -1) {
        perror("Tried to access an invalid file descriptor in write_stream_to_fd()");
        return -1;
    }

    int bytes_written;
    size_t total_written = 0;
    size_t bytes_left = stream.size - total_written;

    while(total_written < stream.size && 
    (bytes_written = write(fd, stream.data + total_written, bytes_left > BUFSIZE ? BUFSIZE : bytes_left)) != -1) {
        total_written += bytes_written;
        bytes_left = stream.size - total_written;

        if(DEBUG) printf("writing %d bytes...\n", bytes_written);
    }

    if(bytes_written == -1) {
        perror("Failed to write bytes to file descriptor - in write_stream_to_fd()");
        return -1;
    }

    if(DEBUG) printf("In total: Wrote %ld bytes to fd %d\n", total_written, fd);

    return (int)total_written;
}

void write_stderr(char * string) {
    stream_t stream;
    stream.size = strlen(string);
    stream.data = string;

    stream_t newline;
    newline.size = 2;
    newline.data = "\n";

    write_stream_to_fd(2, stream);
    write_stream_to_fd(2, newline);
}

int getfdtype(int fd) {
    struct stat statbuf;
    
    if(fstat(fd, &statbuf) == -1) {
        perror("getfdtype - fstat encountered error!");
        return -1;
    }

    int ret_val = -1;
    if(S_ISDIR(statbuf.st_mode)) {
        ret_val = FDTYPE_DIR;
    } else if (S_ISREG(statbuf.st_mode)) {
        ret_val = FDTYPE_REG;
    } else {
        ret_val = FDTYPE_OTHER; 
    }

    return ret_val;
}
