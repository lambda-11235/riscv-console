
#include "vfs.h"

#include "path_parse.h"
#include "util.h"


extern struct entry root;


struct entry* vfs_entry_lookup(const char* name) {
    struct path_parser parser;
    struct entry* current = &root;
    char name_part[MAX_FILE_NAME_LEN];
    int pstatus;

    path_parser_init(&parser, name);

    do {
        pstatus = path_parser_next(&parser, name_part);

        if (pstatus == -1) {
            return NULL;
        } else if (pstatus == 0) {
            if (!(current->perm_flags & DIRECTORY))
                return NULL;

            current = current->edir.child;

            while (current != NULL && !streq(current->name, name_part))
                current = current->next;

            if (current == NULL)
                return NULL;
        }
    } while (pstatus != 1);

    return current;
}



// API Implementation


int open(const char* path, int flags) {
  // TODO
  return -1;
}


int close(int file_id) {
  // TODO
  return -1;
}


uint8_t* memmap(int file_id, size_t offset) {
  // TODO
  return NULL;
}


int seek(int file_id, size_t pos) {
  // TODO
  return -1;
}


ssize_t file_length(int file_id) {
  // TODO
  return -1;
}


ssize_t read(int file_id, uint8_t* buf, size_t amount) {
  // TODO
  return -1;
}


ssize_t write(int file_id, uint8_t* buf, size_t amount) {
  // TODO
  return -1;
}



int dir_open(const char* path) {
  // TODO
  return -1;
}


int dir_close(int dir_id) {
  // TODO
  return -1;
}


int dir_next(int dir_id, char entry_name[MAX_FILE_NAME_LEN]) {
  // TODO
  return -1;
}
