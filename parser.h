#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>
#include <unistd.h>

void parse_ethernet_frame(const unsigned char *buffer, ssize_t len);

#endif
