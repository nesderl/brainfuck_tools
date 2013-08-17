#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

void usage(const char* prog) {
    printf("Usage: %s binaries_file\n", prog);
    exit(-1);
}

#include <time.h>
 
void my_delay(float i)    /*Pause l'application pour i seconds*/
{
        clock_t start,end;
            start=clock();
                while(((end=clock())-start)<=i*CLOCKS_PER_SEC);
}

int main(int argc, char** argv) {
    FILE* in;
    exec_header hdr;
    unsigned short* program;

    unsigned char ram[256];
    unsigned char registers[3];

    memset(ram, 0, 256);
    memset(registers, 0, 3);

    if(argc <= 1) {
        usage(*argv);
    }

    in = fopen(argv[1], "rb");
    if(in == NULL) {
        perror("unable to load binary file");
    }

    fread(&hdr, sizeof(hdr), 1, in);
    
    program = malloc(hdr.binaries_len * sizeof(unsigned short));
    if(program == NULL) {
        fclose(in);
        perror("unable to allocate program memory");
    }

    fread(program, hdr.binaries_len*sizeof(unsigned short), 1, in);
    if(hdr.checksum != checksum((unsigned char*)program, hdr.binaries_len)) {
        free(program);
        fclose(in);
        perror("corrupted binary file");
    }

    fclose(in);

    printf("Program Size = %d\n", hdr.binaries_len);
    printf("Starting programm...\n\n");

    while(registers[IP] != hdr.binaries_len-2) {
//        my_delay(0.125);

        unsigned short inst = program[registers[IP]++];
        unsigned short reg = inst & REGISTER_MASK;
        unsigned short add = inst & ADDRESS_MASK;

        //printf("%d. ", registers[IP]-1);

        switch(inst & INSTRUCTION_MASK) {
            case NOP:
//                printf("NOP\n");
                break;

            case INC:
//                printf("INC %d\n", reg);
                registers[reg]++;
                break;

            case DEC:
//                printf("DEC %d\n", reg);
                registers[reg]--;
                break;

            case WRI:
//                printf("WRI %d\n", reg);
                ram[registers[SP]] = registers[reg];
                break;

            case REA:
//                printf("REA %d\n", reg);
                registers[reg] = ram[registers[SP]];
                break;

            case JMP:
//                printf("JMP %d\n", add);
                registers[IP] = add;
                break;

            case JNZ:
//                printf("JNZ %d\n", add);
                if(registers[AC] != 0)
                    registers[IP] = add;
                break;

            case PRINT:
//                    printf("PRINT \n");
                    printf("%c", registers[AC]);
                    break;

            default:
                printf("FAIL\n");
                break;
        }
        /*
        int i;
        printf("\t\t");
        for(i=0; i<25; i++){
            if(i!=registers[SP])printf("%d ", ram[i]);
            else printf("%d ", registers[AC]);
        }
                    printf("\n");
*/
//        printf("\t[DEBUG] AC=%d, SP=%d, RAM[SP]=%d\n", registers[AC], registers[SP], ram[registers[SP]]);
    }

    printf("\n");
    free(program);
    return 0;
}
