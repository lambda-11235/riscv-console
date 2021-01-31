
#include "vfs.h"

uint8_t* raw_memmap(struct file* self, size_t offset) {
    return self->data + offset;
}

ssize_t raw_read(struct file* self, uint8_t* buf, size_t amount) {
    uint8_t* data = self->data;
    size_t count = 0;

    for (size_t i = 0; i < amount; i++) {
        if (self->pos >= self->length)
            break;

        buf[i] = data[self->pos];
        self->pos++;
        count++;
    }

    return count;
}

ssize_t raw_write(struct file* self, uint8_t* buf, size_t amount) {
    uint8_t* data = self->data;
    size_t count = 0;

    for (size_t i = 0; i < amount; i++) {
        if (self->pos >= self->length)
            break;

        data[self->pos] = buf[i];
        self->pos++;
        count++;
    }

    return count;
}


int init_raw_file_entry(struct entry* ent, char* name, uint8_t* data, size_t length) {
    struct file f = {
        .data = (void*) data,
        .length = length,
        .pos = 0,
        .read = raw_read,
        .write = raw_read
    };

    ent->name = name;
    ent->next = NULL;
    ent->use_flags = 0;
    ent->perm_flags = READ|WRITE;
    ent->efile = f;

    return 0;
}


int init_directory(struct entry* ent, char* name, struct entry* children[], size_t num_ch) {
    struct directory dir = { .child = NULL };

    if (num_ch > 0)
        dir.child = children[0];

    for (size_t i = 1; i < num_ch; i++)
        children[i-1]->next = children[i];

    ent->name = name;
    ent->next = NULL;
    ent->use_flags = 0;
    ent->perm_flags = READ|WRITE|DIRECTORY;
    ent->edir = dir;

    return 0;
}


struct entry sys_dev_video_mode_entry;
struct entry sys_dev_video_entry;
struct entry sys_dev_entry;
struct entry sys_entry;

struct entry root;

int file_system_init(void) {
    init_raw_file_entry(&sys_dev_video_mode_entry,
                        "mode", (uint8_t*) 0x500FF414, 0x4);

    init_directory(&sys_dev_video_entry, "video",
                   (struct entry*[]) {&sys_dev_video_mode_entry}, 1);
    init_directory(&sys_dev_entry, "dev",
                   (struct entry*[]) {&sys_dev_video_entry}, 1);
    init_directory(&sys_entry, "sys",
                   (struct entry*[]) {&sys_dev_entry}, 1);
    init_directory(&root, "",
                   (struct entry*[]) {&sys_entry}, 1);

    return 0;
}
