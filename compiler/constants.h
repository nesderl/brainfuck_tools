#ifndef CONSTANTS_H
#define CONSTANTS_H

// emulator's specs
#define RAM_SIZE 256

// instructions
#define NOP   (unsigned short)0
#define INC   (unsigned short)(1 << 13)
#define DEC   (unsigned short)(2 << 13)
#define SYN   (unsigned short)(3 << 13)
#define JMP   (unsigned short)(4 << 13)
#define JNZ   (unsigned short)(5 << 13)
#define PRINT (unsigned short)(6 << 13)

// registers
#define AC 0
#define IP 1
#define SP 2

#endif
