
#include "util.h"


int streq(const char* str1, const char* str2) {
    while (*str1 != 0 && *str2 != 0) {
        if (*str1 != *str2)
            return 0;

        str1++;
        str2++;
    }

    return 1;
}


int strcpy_(char* dest, const char* source) {
    int i;

    for (i = 0; source[i] != 0; i++)
        dest[i] = source[i];

    return i;
}


int memcpy_(uint8_t* dest, const uint8_t* source, size_t len) {
    for (size_t i = 0; i < len; i++)
        dest[i] = source[i];
}


int u32_to_str(char* buf, uint32_t x) {
    size_t i = 0;
    size_t j = 0;
    char tmp;

    if (x == 0) {
        buf[0] = '0';
        return 0;
    }

    for (i = 0; x > 0; i++) {
        buf[i] = '0' + x%10;
        x /= 10;
    }

    i--;

    while (j < i) {
        tmp = buf[j];
        buf[j] = buf[i];
        buf[i] = tmp;

        j++;
        i--;
    }

    return 0;
}


int i32_to_str(char* buf, int32_t x) {
    if (x < 0) {
        buf[0] = '-';
        buf++;
        x = -x;
    }

    return u32_to_str(buf, x);
}
