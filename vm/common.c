#include "common.h"

unsigned int checksum(unsigned char* data, unsigned int len) {
    unsigned int sum=0;
    while(--len) 
        sum+=data[len];

    return sum;
}
