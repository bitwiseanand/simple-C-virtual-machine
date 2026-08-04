#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"
#include "stringview.h"

/* Instruction names for debugger */

const char *instruction_name[] = {
	"PUSH",
	"POP",
	"ADD",
	"SUB",
	"MUL",
	"DIV",
	"MOD",
	"PRINT",
	"JMP",
	"HALT",
	"JZ",
	"JNZ",
	"EQ",
	"NEQ",
	"LT",
	"GT",
	"LE",
	"GE",
	"DUP",
	"SWAP",
	"OVER",
	"LOAD",
	"STORE",
	"CALL",
	"RET"
};

/* Opcode lookup table */

Opcode opcode_table[] = {
	{{"PUSH", 4},  IN_PUSH},
	{{"POP", 3},   IN_POP},
	{{"ADD", 3},   IN_ADD},
	{{"SUB", 3},   IN_SUB},
	{{"MUL", 3},   IN_MUL},
	{{"DIV", 3},   IN_DIV},
	{{"MOD", 3},   IN_MOD},
	{{"PRINT", 5}, IN_PRINT},
	{{"JMP", 3},   IN_JMP},
	{{"HALT", 4},  IN_HALT},
	{{"JZ", 2},    IN_JZ},
	{{"JNZ", 3},   IN_JNZ},
	{{"EQ", 2},    IN_EQ},
	{{"NEQ", 3},   IN_NEQ},
	{{"LT", 2},    IN_LT},
	{{"GT", 2},    IN_GT},
	{{"LE", 2},    IN_LE},
	{{"GE", 2},    IN_GE},
	{{"DUP", 3},   IN_DUP},
	{{"SWAP", 4},  IN_SWAP},
	{{"OVER", 4},  IN_OVER},
	{{"LOAD",4},   IN_LOAD},
	{{"STORE",5},  IN_STORE},
	{{"CALL",4},   IN_CALL},
	{{"RET",3},    IN_RET}
};


int sv_to_int(String_view sv){

	int value = 0;
	int negative = 0;
	if(sv.data[0] == '-'){
		negative = 1;
		sv.data++;
		sv.size-=1;
	}
	if (sv.size == 0) {
    	printf("Invalid integer\n");
    exit(1);
}
	for (size_t i = 0; i < sv.size; i++)
	{
		if( sv.data[i] >=48 && sv.data[i] <=57)
			value = value * 10 + (sv.data[i] - '0');
		else{
			printf("Not a valid integer  "SV_FMT,SV_ARGS(sv));
			exit(1);
		}
	}
	if(negative) return -1*value;
	return value;
}

/* Tokenize one line */
int resolve_second_token(String_view sv)
{
	

	for (int i = 0; i < symbol_count; i++) {

		if (symbol_table[i].symbol_length == sv.size && memcmp(sv.data, symbol_table[i].symbol_name, sv.size) == 0) {
			return symbol_table[i].address;
		}
	}

	return sv_to_int(sv);
}

Types resolve_instruction_name(String_view opcode)
{
	size_t count = sizeof(opcode_table) / sizeof(opcode_table[0]);

	for (size_t i = 0; i < count; i++)
	{
		if (sv_eq(opcode, opcode_table[i].name))
			return opcode_table[i].instruction;

	}


	printf("Unknown instruction: " SV_FMT "\n", SV_ARGS(opcode));
	exit(1);
}

int hasColon(String_view line){
   if(line.size !=0 && line.data[line.size - 1]  == ':') return 1;
   return 0;
}

int tokenize(String_view line, int count,int pass)
{
	trim_left(&line);
	trim_right(&line);

	if (line.size == 0)
		return 0;
	for (size_t i = 0; i < line.size; i++) {
		if (line.data[i] == '#') {
			line.size = i;
			break;
		}
	}

	trim_right(&line);
	if (line.size == 0)
		return 0;

	String_view opcode = chop_by_delim(&line, ' ');

	trim_left(&line);

	if (pass == 1) {

		//resolve the variables


      
		//resolving for loop conditions
		if(hasColon(opcode)) {
			opcode.size-=1;

			memcpy(symbol_table[symbol_count].symbol_name,opcode.data,opcode.size);
			symbol_table[symbol_count].address = count;
			symbol_table[symbol_count].symbol_name[opcode.size] = '\0';
			symbol_table[symbol_count].symbol_length=opcode.size;
			symbol_count++;


			opcode = chop_by_delim(&line,' ');
			opcode = trim_left(&opcode);
			if(opcode.size > 0) {
				return 1;
			}
			return 0;
		}
        else if( sv_eq(opcode,sv("VAR"))){
            opcode = chop_by_delim(&line,' ');
            opcode = trim_left(&opcode);
            
           	memcpy(symbol_table[symbol_count].symbol_name, opcode.data, opcode.size);
			symbol_table[symbol_count].symbol_name[opcode.size] = '\0';
			symbol_table[symbol_count].symbol_length = opcode.size;
			symbol_table[symbol_count].address = variable_counter++;

			symbol_count++;

			return 0;
        }



	}

	if(pass == 2) {

		if(hasColon(opcode) ) {

            opcode = chop_by_delim(&line,' ');
            if(opcode.size == 0 ) return 0;

        }
           
        if(sv_eq(opcode,sv("VAR"))){
            return 0;
        }

            
          
			ip[count].INSTRUCTION = resolve_instruction_name(opcode);
			ip[count].OPERAND = 0;

			if (line.size > 0)
			{

					ip[count].OPERAND = resolve_second_token(line);
					ip[count].has_operand = true;
			}
			
            
}
return 1;
}

