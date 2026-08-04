#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "stringview.h"
#include<stdbool.h>
#define PROGRAM_SIZE 100



typedef enum {
    IN_PUSH,
    IN_POP,
    IN_ADD,
    IN_SUB,
    IN_MUL,
    IN_DIV,
    IN_MOD,
    IN_PRINT,
    IN_JMP,
    IN_HALT,
    IN_JZ,
    IN_JNZ,
    IN_EQ,
    IN_NEQ,
    IN_LT,
    IN_GT,
    IN_LE,
    IN_GE,
    IN_DUP,
    IN_SWAP,
    IN_OVER,
    IN_LOAD,
    IN_STORE,
    IN_CALL,
    IN_RET
} Types;



typedef struct {
    Types INSTRUCTION;
    int OPERAND;
    bool has_operand;
} Instruction;



typedef struct {
    String_view name;
    Types instruction;
} Opcode;

typedef struct {
    char symbol_name[20];
    size_t symbol_length;
    int address;
}Symbol;




/* Instruction array (defined in main.c) */
extern Instruction ip[PROGRAM_SIZE];

/* Instruction names (defined in tokenizer.c) */
extern const char *instruction_name[];

/* Opcode lookup table (defined in tokenizer.c) */
extern Opcode opcode_table[];
extern int symbol_count;
extern Symbol symbol_table[];
extern int variable_counter;

/* Convert opcode text to enum */
Types resolve_instruction_name(String_view opcode);

/* Parse one line into ip[count] */
int tokenize(String_view line, int count,int pass);

#endif /* TOKENIZER_H */