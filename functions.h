#ifndef FUNCTIONS
#define FUNCTIONS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdint.h>


/* Valid Errors */
#define CORRECT 1
#define FUNCTIONERROR 2
#define TYPEOPERAND 3
#define NOTNUMBER 4
#define TOOMUCH 5
#define OUTOFRANGE 6
#define NEEDMORE 7
#define IMMERROR 8
#define NOTSYMBOL 9
#define ERROR 10
#define NOTQUOT 11
#define MISSCOMMA 11
#define DOUBLECOMMA 12
#define THEMAXIS80 13
#define SYMBOLTOOLONG 14
#define THEMAXIS12 15
#define SYMBOLNOTEXIST 16
#define SYMBOLEXIST 17
#define EMPTYSYMBOL 18


/* Random */
#define COMMA ','
#define SPACE ' '
#define NL '\n'
#define TE '\0'
#define TWOPOINTS 58
#define DOT 46
#define POINTCOMMA ';'
#define QUOT '"'
#define TAB '\t'
#define DOLLAR '$'

/* Commands Definition */
#define COMMAND 1
#define DATA 2
#define EXTERN 31

#define ADD 0
#define SUB 1
#define AND 2
#define OR 3
#define NOR 4
#define MOVE 5
#define MVHI 6
#define MVLO 7
#define ADDI 8
#define SUBI 9
#define ANDI 10
#define ORI 11
#define NORI 12
#define BNE 13
#define BEQ 14
#define BLT 15
#define BGT 16
#define LB 17
#define SB 18
#define LW 19
#define SW 20
#define LH 21
#define SH 22
#define JMP 23
#define LA 24
#define CALL 25
#define STOP 26
#define ASCIZ 27
#define DB 28
#define DH 29
#define DW 30
#define ENTRY 32


/* Valid List */
typedef struct validList* pValid;

typedef struct validList{
	char error[50];
	int value;
	pValid next;
}ValidList;


/* Data Table */
typedef struct DataTable* pData;

typedef struct DataTable{
	char symbol[32];
	int value;
	int type;
	pData next;
}DataTable;


/* Functions Table */

typedef struct opcodeTable
{
	int name;
	char type;
	int funct;
	int opcode;
}opCode;


/* Functions Heads */

/* increses x by power y */
int power(int x, int y);

/* reverse the entered string */
void reverse(char* command);

/* convert 4 binary digit into 1 hex digit */
char hexFourBitConverter(char* bin);

/* convert string of binary digits to hex digits form */
void binToHex(char *n, int stringLen, char* output1);

/* convert string of dec digits to bin digits form */
const char *decimalToBin(int n, int b);

/* convert string of dec digits to bin  2 complement digits form */
const char *decimalToBin2n(int n, int b);

/* This method iterates through all the commands and translates them into machine language */
void transform(FILE* file, char* fileName);

/* the method check if there is : on the string */
int checkTwoPoints(char* command);

/* this method return the type of the command (add,sub...) */
int type(char* command);

/* this method return the size of the quote in the asciz */
int ascizSize(char* command);

/* return the number of commas in the command */
int countComma(char* command);

/* struct the data table */
void structDataTable(FILE* file, char* fileName);

/* return the address of symbol */
int lineNumber(char* symbol);

/* return the type of the command inside the symbol */ 
int typeSymbol(char* symbol);

/* push to data table new symbol */
void pushToData(pData* data, char* symbol, int value, int type, int count1);

/* gets the label name */
void getSymbol(char* command, char* symbol);

/* extract line from the file */
void getLine(FILE* file, char* command);

/* struct the op code table */
void structOpcodeTable(struct opcodeTable* opcode);

/* extract the variables in command type R */
void variablesR(char* command, int function, int *arr);
/* translate command type R */
int typeR(char* command, FILE* file);

/* extract the variables in command type I */
void variablesI(char* command, int function, int *arr,int count);

/* translate command type I */
int typeI(char* command, int count, FILE* file);

/* extract the variables in command type J */
void variablesJ(char* command, int function, int *arr,int count);

/* translate command type J */
int typeJ(char* command, int count, FILE* file);

/* translate command type asciz */
void typeAsciz(char* command, FILE* file);

/* translate command type db */
void typeDb(char* command, FILE* file);

/* translate command type dh */
void typeDh(char* command, FILE* file);

/* translate command type dw */
void typeDw(char* command, FILE* file);

/* check if there is comma in command */
int checkComma(char* command);

/* return the length of command */
int stringLength(char* command);

/* check if char is alphaBet or not */
int alphaBet(char a);

/* check if symbol is valid */
int validSymbol(char* symbol);

/* check if number is in range for 16 bits number */
int inRange16(char* number);

/* check if number is in range for 8 bits number */
int inRange8(char* number);

/* check if number is in range for 32 bits number */
int inRange32(char* number);

/* check if number is between 1-31 */
int inRange(char* number);

/* extract number for the command */
void getNumber(char** p , char* number);

/* check if there is double comma in the command */
int doublePsik(char* command);

/* check if a is a number or not */
int checkNumber(char* a);

/* check if command is valid or not */
int isValid(char* command);

/* struct table that contains all the errors */
void structValidList(FILE* file, char* fileName);

/* push new error into error table */
void pushToValid(pValid* valid, char* error, int value);

/* 4 methods write the input into the file */
void writeOnFile(FILE* file, char* input);
void writeNumber4(FILE* file, int input);
void writeNumber(FILE* file, int input);
void writeOnEntry(FILE* file, char* input);

/* main method */
void run(char* fileName);




#endif














