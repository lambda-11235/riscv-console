
#include <stddef.h>

#ifndef _PATH_PARSE_H_
#define _PATH_PARSE_H_

struct path_parser {
    size_t pos;
    const char* source;
};

/**
 * Initializes a path parser.
 *
 * @param parser the parser to initialize
 * @param name the path the parse
 *
 * @return 0 on success, -1 on failure
 */
int path_parser_init(struct path_parser* parser, const char* name);

/**
 * Gets the next part of the path.
 *
 * @param parser the parser
 * @param buf the buffer to store the next part in
 *
 * @return 0 if there is more parts, 1 if there are no more parts, and
 * -1 on error.
 */
int path_parser_next(struct path_parser* parser, char* buf);

#endif /* _PATH_PARSE_H_ */
