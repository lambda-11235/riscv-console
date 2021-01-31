
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#ifndef _VFS_H_
#define _VFS_H_

typedef int32_t ssize_t;

#define MAX_FILE_NAME_LEN 256

// Use flags, tracking how files are open
#define READ 1
#define WRITE 2
#define OPEN 4

// Permission flags, inherent properties of the file
// READ
// WRITE
#define DIRECTORY 4

struct entry;

struct directory {
    struct entry* child;
};

struct file {
    void* data;
    size_t length;
    size_t pos;

    uint8_t* (*memmap)(struct file* self, size_t offset);
    ssize_t (*read)(struct file* self, uint8_t* buf, size_t amount);
    ssize_t (*write)(struct file* self, uint8_t* buf, size_t amount);
};

struct entry {
    char* name;
    struct entry* next;
    uint16_t use_flags;
    uint16_t perm_flags;

    union {
        struct directory edir;
        struct file efile;
    };
};


int file_system_init(void);

int open(const char* path, int flags);
int close(int file_id);
uint8_t* memmap(int file_id, size_t offset);
int seek(int file_id, size_t pos);
ssize_t file_length(int file_id);
ssize_t read(int file_id, uint8_t* buf, size_t amount);
ssize_t write(int file_id, uint8_t* buf, size_t amount);

int dir_open(const char* path);
int dir_close(int dir_id);
int dir_next(int dir_id, char entry_name[MAX_FILE_NAME_LEN]);

#endif /* _VFS_H_ */
