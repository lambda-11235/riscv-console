
#include "vfs.h"

#include "fault.h"
#include "path_parse.h"
#include "util.h"


extern struct entry root;

// TODO: This should be dynamically sized, but heap allocation isn't
// implemented yet.
#define MAX_OPEN_FILES 256
struct entry* open_files[MAX_OPEN_FILES];


struct entry* vfs_entry_lookup(const char* path) {
    struct path_parser parser;
    struct entry* current = &root;
    char path_part[MAX_FILE_NAME_LEN];
    int pstatus;

    path_parser_init(&parser, path);

    do {
        pstatus = path_parser_next(&parser, path_part);

        if (pstatus == -1) {
            return NULL;
        } else if (pstatus == 0) {
            if (!(current->perm_flags & DIRECTORY))
                return NULL;

            current = current->edir.child;

            while (current != NULL && !streq(current->name, path_part))
                current = current->next;

            if (current == NULL)
                return NULL;
        }
    } while (pstatus != 1);

    return current;
}



// API Implementation


int open(const char* path, int flags) {
    size_t file_id = 0;
    struct entry* ent;

    // Find an open file id.
    while (open_files[file_id] != NULL) {
        file_id++;

        if (file_id >= MAX_OPEN_FILES)
            return -1;
    }

    ent = vfs_entry_lookup(path);

    if (ent == NULL || (ent->perm_flags & DIRECTORY)
        || (ent->use_flags & OPEN)) {
        return -1;
    }

    if ((flags & READ) && !(ent->perm_flags & READ))
        return -1;

    if ((flags & WRITE) && !(ent->perm_flags & WRITE))
        return -1;

    ent->use_flags = flags | OPEN;
    open_files[file_id] = ent;

    return file_id;
}


int close(int file_id) {
    int retval = 0;

    if (file_id < 0 || file_id >= MAX_OPEN_FILES || open_files[file_id] == NULL)
        return -1;

    if (!(open_files[file_id]->use_flags & OPEN))
        fault("File in open file table, but open permission not set");

    open_files[file_id]->use_flags &= ~OPEN;
    open_files[file_id] = NULL;

    return retval;
}


uint8_t* memmap(int file_id, size_t offset) {
    struct entry* ent = open_files[file_id];
    if (ent == NULL)
        return NULL;
    if (!((ent->use_flags & READ) || !(ent->use_flags & WRITE)))
        return NULL;
    if (ent->perm_flags & DIRECTORY)
        return NULL;

    return ent->efile.memmap(&ent->efile, offset);
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
