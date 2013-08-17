#include "compiler.h"

// LOCAL UTILITIES
struct Stack {
    struct Stack* next;
    unsigned short address;
};
typedef struct Stack Stack;

void push(Stack** stack, unsigned short address) {
    Stack* new = malloc(sizeof(Stack));
    if(new == NULL) {
        perror("while allocating compiler resources");
    }
    
    new->address = address;
    new->next = *stack;
    *stack = new;

}

unsigned short pop(Stack** stack) {
    Stack* old = *stack;
    unsigned short address = old->address;
    
    *stack = old->next;
    free(old);
    return address;
}

// COMPILER.H
unsigned char statement_len(unsigned short s) {
    if(s=='>' || s=='<')
        return 3;
    if(s=='[')
        return 2;
    return 1;
}

unsigned int binary_len(unsigned char* source) {
    unsigned int size=0;

    do {
        size += statement_len(*source);
    } while(*(source++));

    return size;
}


void pbin(unsigned short b){
    int i=16;
    while(--i>=0)
        printf("%d", (b&(1<<i))>>i);

    printf("\t(%u)\n", b);
}

unsigned short* compile(unsigned char* source, const char* filename) {
    FILE* out = NULL;

    unsigned short* binaries;
    unsigned short binaries_size;

    printf("MASKS:\n");
    printf("\tINSTRUCTION MASK\t"); pbin(INSTRUCTION_MASK);
    printf("\tREGISTER MASK\t\t"); pbin(REGISTER_MASK);
    printf("\tADDRESS MASK\t\t"); pbin(ADDRESS_MASK);
    printf("\n");

    printf("INSTRUCTION SET:\n");
    printf("\tNOP\t"); pbin(NOP);
    printf("\tINC\t"); pbin(INC);
    printf("\tDEC\t"); pbin(DEC);
    printf("\tWRI\t"); pbin(WRI);
    printf("\tREA\t"); pbin(REA);
    printf("\tJMP\t"); pbin(JMP);
    printf("\tJNZ\t"); pbin(JNZ);
    printf("\tPRINT\t"); pbin(PRINT);
    printf("\n");

    printf("REGISTERS:\n");
    printf("\tSP\t"); pbin(SP);
    printf("\tAC\t"); pbin(AC);
    printf("\tIP\t"); pbin(IP);
    printf("\n");
 
   // create binaries buffer
    binaries_size = binary_len(source) + 1; // add at the end of the program an infinite loop
    if(binaries_size <= 1) {
        perror("invalid source code");
    }

    binaries = malloc(binaries_size * sizeof(unsigned short));
    if(binaries == NULL) {
        perror("malloc failled while compiling");
    }

    memset(binaries, 0, binaries_size*sizeof(unsigned short));

    // "compile"
    unsigned char* c = source;
    unsigned short p = 0;
    Stack* loops = NULL;

    do {
        printf("%c\n\t", *c);
        printf("%d. ", p);

        switch(*c) {
            case '>':
                binaries[p] = WRI|AC;
                printf("WRI AC\t");
                pbin(binaries[p]);

                binaries[++p] = INC|SP;
                printf("\t%d. INC SP\t", p);
                pbin(binaries[p]);

                binaries[++p] = REA|AC;
                printf("\t%d. REA AC\t", p);
                pbin(binaries[p]);
                break;

            case '<':
                binaries[p] = WRI|AC;
                printf("WRI AC\t");
                pbin(binaries[p]);

                binaries[++p] = DEC|SP;
                printf("\t%d. DEC SP\t", p);
                pbin(binaries[p]);
                
                binaries[++p] = REA|AC;
                printf("\t%d. REA AC\t", p);
                pbin(binaries[p]);
                break;

            case '+':
                binaries[p] = INC|AC;
                printf("INC AC\t");
                pbin(binaries[p]);
                break;

            case '-':
                binaries[p] = DEC|AC;
                printf("DEC AC\t");
                pbin(binaries[p]);             
                break;

            case '.':
                binaries[p] = PRINT;
                printf("PRINT\t");
                pbin(binaries[p]);
                break;

            case '[':
                binaries[p] = JNZ|(p+2);
                printf("JNZ %u\t", p+2);
                pbin(binaries[p]);

                push(&loops, p);

                binaries[++p] = JMP;
                printf("\t%d. JMP ?\t", p);
                pbin(binaries[p]);

                break;

            case ']':
                 {
                     unsigned short loop_start = pop(&loops);

                     binaries[p] = JMP|loop_start;
                     printf("JMP %u\t", loop_start);
                     pbin(binaries[p]);

                     binaries[loop_start+1] |= p+1;
                     printf("\t\t%d. JMP %u\t", loop_start+1, p+1);
                     pbin(binaries[loop_start+1]);
                     break;
                 }

            default: break;
        }
        p++;
    } while(*(++c));
    printf("\n");

    binaries[binaries_size-1] = JMP|(binaries_size-1); 

    if(loops != NULL) {
        free(binaries);
        perror("invalid source code loops"); 
    }

    out = fopen(filename, "wb+");
    if(out == NULL) {
        free(binaries);
        perror("invalid output file");
    }

    exec_header hdr;
    hdr.binaries_len = binaries_size;
    hdr.checksum = checksum((unsigned char*)binaries, binaries_size);

    fwrite(&hdr, sizeof(exec_header), 1, out);
    fwrite(binaries, binaries_size*sizeof(unsigned short), 1, out);

    fclose(out);
    return binaries;
}
