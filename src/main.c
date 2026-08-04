#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stringview.h"
#include "tokenizer.h"
#define STACK_SIZE 1024
#define PROGRAM_SIZE 100
#define STORAGE_SIZE 1024
#define CALL_STACK_SIZE 1024
#define SYMBOL_TABLE_SIZE 100
Instruction ip[PROGRAM_SIZE];

int stack[STACK_SIZE];
int sp = 0;
int symbol_count=0;
Symbol symbol_table[SYMBOL_TABLE_SIZE];
int variable_storage[STORAGE_SIZE];
int variable_counter = 0 ;
int call_stack[CALL_STACK_SIZE];
int call_stack_counter=-1;

void debug_mode_fn(int pc) {

	printf("\n-------------------------------\n");
	printf("PC: %3d\n", pc );
	printf("Opcode %s",instruction_name[ip[pc].INSTRUCTION]);
	printf("\nstack is \n");
	for(int i=0; i<sp; i++) {
		printf("%3d",stack[i]);
	}
	printf("\nPress Enter to step");
	fflush(stdout);
	getchar();
}

void push(int value)
{
	if (sp >= STACK_SIZE) {
		printf("Stack Overflow\n");
		exit(1);
	}

	stack[sp++] = value;
}
int peek() {
	if(sp<=0) {
		printf("stack undeflow");
		exit(1);
	}
	return stack[sp-1];
}
int pop()
{
	if (sp <= 0) {
		printf("Stack Underflow\n");
		exit(1);
	}

	return stack[--sp];
}
int count = 0; // counts number of lines of code
int pass = 1; // two pass

void load_program() {

	FILE *fp = fopen("program.txt", "r");
	char line[100];

	if (fp == NULL) {
		printf("Cannot open program.txt\n");
		exit(1);
	}


	while (fgets(line, sizeof(line), fp))
	{
		String_view read_a_line = sv(line);
		int sucess = tokenize(read_a_line,count,pass);
		if(sucess)
			count++;

	}
	rewind(fp);
	count =0;
	pass+=1;

	while (fgets(line, sizeof(line), fp))
	{
		String_view read_a_line = sv(line);

		int sucess = tokenize(read_a_line,count,pass);
		if(sucess)
			count++;

	}


	fclose(fp);


}

int validate_pc(int pc) {
	if(pc < count) {
		return 1;
	}
	return 0;
}


int run_program(int debug_mode)
{
	int pc = 0;

	while (pc < count)
	{
		if (debug_mode)
			debug_mode_fn(pc);

		switch (ip[pc].INSTRUCTION)
		{
		case IN_PUSH:
			push(ip[pc].OPERAND);
			break;

		case IN_POP:
			pop();
			break;

		case IN_ADD:
		{
			int a = pop();
			int b = pop();
			push(b + a);
			break;
		}

		case IN_SUB:
		{
			int a = pop();
			int b = pop();
			push(b - a);
			break;
		}

		case IN_MUL:
		{
			int a = pop();
			int b = pop();
			push(b * a);
			break;
		}

		case IN_DIV:
		{
			int a = pop();
			int b = pop();

			if (a == 0)
			{
				printf("Cannot divide by zero\n");
				exit(1);
			}

			push(b / a);
			break;
		}

		case IN_MOD:
		{
			int a = pop();
			int b = pop();

			if (a == 0)
			{
				printf("Cannot modulo by zero\n");
				exit(1);
			}

			push(b % a);
			break;
		}

		case IN_PRINT:
			printf("%d\n", peek());
			break;

		case IN_JMP:
			if (validate_pc(ip[pc].OPERAND))
			{
				pc = ip[pc].OPERAND;
				continue;
			}
			else
			{
				printf("Invalid jump address\n");
				exit(1);
			}

		case IN_HALT:
			printf("Program Finished\n");
			return 0;

		case IN_JZ:
			if (peek() == 0 && validate_pc(ip[pc].OPERAND))
			{
				pc = ip[pc].OPERAND;
				continue;
			}
			break;

		case IN_JNZ:
			if (peek() != 0 && validate_pc(ip[pc].OPERAND))
			{
				pc = ip[pc].OPERAND;
				continue;
			}
			break;

		case IN_EQ:
		{
			int a = pop();
			int b = pop();
			push(b == a);
			break;
		}

		case IN_NEQ:
		{
			int a = pop();
			int b = pop();
			push(b != a);
			break;
		}

		case IN_LT:
		{
			int a = pop();
			int b = pop();
			push(b < a);
			break;
		}

		case IN_GT:
		{
			int a = pop();
			int b = pop();
			push(b > a);
			break;
		}

		case IN_LE:
		{
			int a = pop();
			int b = pop();
			push(b <= a);
			break;
		}

		case IN_GE:
		{
			int a = pop();
			int b = pop();
			push(b >= a);
			break;
		}

		case IN_DUP:
		{
			push(peek());
			break;
		}

		case IN_SWAP:
		{
			int a = pop();
			int b = pop();

			push(a);
			push(b);
			break;
		}

		case IN_OVER:
		{
			int a = pop();
			int b = pop();

			push(b);
			push(a);
			push(b);
			break;
		}

		case IN_LOAD:
		{


			push(variable_storage[ip[pc].OPERAND]);
			break;
		}

		case IN_STORE:
		{


			variable_storage[ip[pc].OPERAND] = pop();
			break;
		}
		case IN_CALL:
		{
			if (call_stack_counter >= CALL_STACK_SIZE - 1) {
				printf("Call stack overflow\n");
				exit(1);
			}

			call_stack[++call_stack_counter] = pc + 1;
			pc = ip[pc].OPERAND;
			continue;
		}

		case IN_RET:
		{
			if (call_stack_counter < 0) {
				printf("RET without CALL\n");
				exit(1);
			}

			pc = call_stack[call_stack_counter--];
			continue;
		}

		default:
			printf("Invalid Instruction\n");
			return 0;
		}

		pc++;
	}

	return 0;
}
int main(int argc,char *argv[])
{
	int debug_mode = 0;
	if(argc > 1 && strcmp(argv[1],"-debug") == 0)
		debug_mode = 1;




	load_program();
	run_program(debug_mode);


	return 0;
}