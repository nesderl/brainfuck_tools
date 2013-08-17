#include <stdio.h>

#include "compiler.h"
#include "common.h"

void usage(const char* prog) {
    printf("Usage: %s source dest_file\n", prog);
    exit(-1);
}

int main(int argc, char** argv) {
    if(argc < 2) usage(argv[0]);
    free(compile(argv[1], argv[2]));
    return 0;
}
