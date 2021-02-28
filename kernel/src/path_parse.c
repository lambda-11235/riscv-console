
#include "path_parse.h"


int path_parser_init(struct path_parser* parser, const char* name) {
    parser->pos = 0;
    parser->source = name;

    return 0;
}


int path_parser_next(struct path_parser* parser, char* buf) {
    size_t i = 0;
    char ch;

    while (parser->source[parser->pos] == '/')
        parser->pos++;

    ch = parser->source[parser->pos];

    if (ch == 0)
        return 1;

    while (ch != 0 && ch != '/') {
        buf[i] = ch;
        parser->pos++;
        i++;

        ch = parser->source[parser->pos];
    }

    buf[i] = 0;

    return 0;
}
