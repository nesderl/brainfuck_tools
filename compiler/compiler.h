#ifndef COMPILER_H
#define COMPILER_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "common.h"

unsigned char statement_len(unsigned short statement);

unsigned short* compile(unsigned char* source, const char* dest);

unsigned int binary_len(unsigned char* source);

#endif
