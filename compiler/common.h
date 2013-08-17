#ifndef COMMON_H
#define COMMON_H

#define NOP   (unsigned short)0
#define INC   (unsigned short)(1 << 13)
#define DEC   (unsigned short)(2 << 13)
#define WRI   (unsigned short)(3 << 13)
#define REA   (unsigned short)(4 << 13)
#define JMP   (unsigned short)(5 << 13)
#define JNZ   (unsigned short)(6 << 13)
#define PRINT (unsigned short)(7 << 13)

#define SP (unsigned short)0
#define AC (unsigned short)1
#define IP (unsigned short)2

#define INSTRUCTION_MASK (unsigned short)(7<<13)
#define REGISTER_MASK (unsigned short)3
#define ADDRESS_MASK (unsigned short)((1<<13)-1)

struct exec_header {
    unsigned int binaries_len;
    unsigned int checksum;
};
typedef struct exec_header exec_header;

unsigned int checksum(unsigned char* data, unsigned int len);

unsigned short extract_instruction(unsigned short bin);

unsigned short extract_address(unsigned short bin);

unsigned char extract_register(unsigned short bin);

#endif
