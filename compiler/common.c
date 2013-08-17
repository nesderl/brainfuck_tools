#include "common.h"

unsigned int checksum(unsigned char* data, unsigned int len) {
    unsigned int sum=0;
    while(--len) 
        sum+=data[len];

    return sum;
}

unsigned short extract_instruction(unsigned short bin) {
    return (INSTRUCTION_MASK&bin);
}

unsigned short extract_address(unsigned short bin) {
    return (bin&ADDRESS_MASK);
}

unsigned char extract_register(unsigned short bin) {
    return (bin&REGISTER_MASK);
}
