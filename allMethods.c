#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdint.h>


pValid validList = NULL;

void freeValid()
{
	pValid p;
	if(validList == NULL)
	{
		return;
	}
	p = validList;
	validList = validList->next;
	while(validList != NULL)
	{
		free(p);
		p = validList;
		validList = validList->next;
	}
	free(p);
}






int power(int x, int y){ 
    int result = 1;
    while(y > 0){
        result = result*x;
        y--;
    }
    return result;
}



void reverse(char* command) 
{
	char save[100] = "";
	char* p = command;
	char word[100] = "";
	while( (*p) != TE )
	{
		strcpy(save, word);
		strcpy(word,"");
		strncat(word, p, 2);
		strcat(word,save);
		p += 2;
	}
	strcpy(command,word);
}


char hexFourBitConverter(char* bin)
{
	char hex;
	if (strcmp(bin, "0000") == 0){hex = '0';}
	else if (strcmp(bin, "0001") == 0){hex = '1';}
	else if (strcmp(bin, "0010") == 0){hex = '2';}
	else if (strcmp(bin, "0011") == 0){hex = '3';}
	else if (strcmp(bin, "0100") == 0){hex = '4';}
	else if (strcmp(bin, "0101") == 0){hex = '5';}
	else if (strcmp(bin, "0110") == 0){hex = '6';}
	else if (strcmp(bin, "0111") == 0){hex = '7';}
	else if (strcmp(bin, "1000") == 0){hex = '8';}
	else if (strcmp(bin, "1001") == 0){hex = '9';}
	else if (strcmp(bin, "1010") == 0){hex = 'A';}
	else if (strcmp(bin, "1011") == 0){hex = 'B';}
	else if (strcmp(bin, "1100") == 0){hex = 'C';}
	else if (strcmp(bin, "1101") == 0){hex = 'D';}
	else if (strcmp(bin, "1110") == 0){hex = 'E';}
	else if (strcmp(bin, "1111") == 0){hex = 'F';}
	else {printf("Error in binToHex\n");}
	return hex;
}



void binToHex(char *n, int stringLen, char* output1)
{
	char* p = n;
	char current[5] = "";
	char output[32] = "";
	char save;
	int i;
	for(i = 0 ; i < stringLen/4 ; i++)
	{
		strncat(current, p, 4);
		p = p+4;
		save = hexFourBitConverter(current);
		strncat(output, &save, 1);
		strcpy(current,"");
	}
	strcpy(output1, output);
} 



const char *decimalToBin(int n, int b)
{
	int number;
	int current = b-1;
    	static char output[32] = "";
	int i = 0;
	for(;i<b;i++)
	{
		output[i] = '0';
	}
	number = n;
	if(number < 0){number = number*-1;}
	while(number != 0)
	{
		if(number%2 == 1){output[current] = '1';}
		current--;
		number = number/2;
	}
	return output;
} 


const char* decimalToBin2n(int n, int b)
{
	int64_t min = -2147483647-1;
	int current = b-1;
	int number = n;
	static char output[32] = "";
	int i = 0;
	for(;i<b;i++)
	{
		output[i] = '0';
	}
	if(b == 32 && n == min)
	{
		output[0] = '1';
		return output;
	}
	if(number < 0){number = number*-1;}
	while(number != 0)
	{
		if(number%2 == 1){output[current] = '1';}
		current--;
		number = number/2;
	}
	if(n < 0)
	{
		current = b-1;
		while(output[current] != '1')
		{
			current--;
		}
		current--;
		while(current != -1)
		{
			if(output[current] == '1'){output[current] = '0';}
			else if(output[current] == '0'){output[current] = '1';}
			current--;
		}
	}
	return output;
} 


void transform(FILE* file, char* fileName)
{
	char command[400] = "";
	int countCommand = 0;
	int countData = 0;
	FILE* fileEnt;
	FILE* fileExt;
	FILE* fileObj;
	int count = 100;
	char* p = fileName;
	char name[100] = "";

	char nameEnt[100] = "";
	char nameExt[100] = "";
	char nameObj[100] = "";

	while(p[0] != '.')
	{
		strncat(name, p, 1);
		p++;
	}

	strcpy(nameEnt,name);
	strcpy(nameExt,name);
	strcpy(nameObj,name);
	
	strncat(nameEnt,".ent",4);
	strncat(nameExt,".ext",4);
	strncat(nameObj,".ob",3);

	fileEnt = fopen(nameEnt,"w");
	fileExt = fopen(nameExt,"w");
	fileObj = fopen(nameObj,"w");
	

	getLine(file, command);
	while(strcmp(command,"EOF") != 0)
	{
		int commandType = type(command);
		if(commandType >= ADD && commandType <= STOP)
		{
			countCommand +=4;
		}
		if(commandType == ASCIZ)
		{
			countData += ascizSize(command);
		}
		if(commandType == DB)
		{
			countData += countComma(command);
		}
		if(commandType == DH)
		{
			countData += countComma(command)*2;
		}
		if(commandType == DW)
		{
			countData += countComma(command)*4;
		}
		getLine(file, command);
	}
	writeNumber(fileObj, countCommand);
	writeNumber(fileObj, countData);

	fclose(file);
	file = fopen(fileName,"r");
	getLine(file, command);
	while(strcmp(command,"EOF") != 0)
	{
		int commandType = type(command);
		if(commandType >= ADD && commandType <= MVLO)
		{
			typeR(command, fileObj);
			count += 4;
		}
		if(commandType >= ADDI && commandType <= SH)
		{
			if(commandType >= BNE && commandType <= BGT)
			{
				char currentSymbol[50] = "";
				int save;
				char save2[10] = "";
				int countComma = 0; 
				char* pointer = command;
				while(countComma != 2)
				{
					if(pointer[0] == COMMA)
					{
						countComma++;
					}
					pointer++;
				}
				while(pointer[0] != NL)
				{
					strncat(currentSymbol, pointer, 1);
					pointer++;
				}

				save = typeSymbol(currentSymbol); 
				if(save == EXTERN)
				{
					strncat(currentSymbol," ",1);
					sprintf(save2, "%d", count);
					strncat(currentSymbol,save2,4);
					writeOnEntry(fileExt, currentSymbol);
				}
			}
			typeI(command, count, fileObj);
			count += 4;
		}
		if(commandType >= JMP && commandType <= STOP)
		{
			if(commandType != STOP)
			{
				char currentSymbol[50] = "";
				int save;
				char save2[10] = ""; 
				char* pointer = command;
				while(pointer[0] != SPACE)
				{
					pointer++;
				}
				pointer++;
				if(pointer[0] != DOLLAR)
				{
					while(pointer[0] != NL)
					{
						strncat(currentSymbol, pointer, 1);
						pointer++;
					}
					save = typeSymbol(currentSymbol); 
					if(save == EXTERN)
					{
						strncat(currentSymbol," ",1);
						sprintf(save2, "%d", count);
						strncat(currentSymbol,save2,4);
						writeOnEntry(fileExt, currentSymbol);
					}
				}
			}
			typeJ(command, count, fileObj);
			count += 4;
		}
		if(commandType == ENTRY)
		{
			char currentSymbol[50] = "";
			int save;
			char save2[10] = "";
			
			char* pointer = command;
			while(pointer[0] != SPACE)
			{
				pointer++;
			}
			pointer++;
			while(pointer[0] != NL)
			{
				strncat(currentSymbol, pointer, 1);
				pointer++;
			}

			save = lineNumber(currentSymbol);
			strncat(currentSymbol, " ",1);
			sprintf(save2, "%d", save);
			strncat(currentSymbol, save2, 4);
			writeOnEntry(fileEnt, currentSymbol);
		}
		getLine(file, command);
	}
	fclose(file);
	file = fopen(fileName,"r");
	getLine(file, command);
	while(strcmp(command,"EOF") != 0)
	{
		int commandType = type(command);
		if(commandType == ASCIZ)
		{
			typeAsciz(command, fileObj);
			count += ascizSize(command);
		}
		if(commandType == DB)
		{
			typeDb(command, fileObj);
			count += countComma(command);
		}
		if(commandType == DH)
		{
			typeDh(command, fileObj);
			count += countComma(command)*2;
		}
		if(commandType == DW)
		{
			typeDw(command, fileObj);
			count += countComma(command)*4;
		}
		getLine(file, command);
	}

}


int checkTwoPoints(char* command){ 
	char *s = command;
	while(*s != NL && *s != TE){
		if(*s == TWOPOINTS){
			return 1;
		}
		s++;
	}
	return 0;
}


int type(char* command)
{
	char word[10] = "";
	int check = checkTwoPoints(command);
	char *s = command;
	if(check == 1){
		while(*s != TWOPOINTS){
			s++;
		}
		s++;
	}
	if(*s == DOT)
	{
		s++;
		while( (*s) != SPACE && (*s) != NL && (*s) != TE){
			strncat(word,s,1);
			s++;
		}
		if (strcmp(word, "asciz") == 0){ return ASCIZ; }
		else if (strcmp(word, "db") == 0){ return DB; }
		else if (strcmp(word, "dw") == 0){ return DW; }
		else if (strcmp(word, "dh") == 0){ return DH; }
		else if (strcmp(word, "extern") == 0){ return EXTERN; }
		else if (strcmp(word, "entry") == 0){ return ENTRY; }
		return -1;
		
	}
	while( (*s) != SPACE && (*s) != NL && (*s) != TE ){
		strncat(word,s,1);
		s++;
	}
	if (strcmp(word, "add") == 0){ return ADD; }
	else if (strcmp(word, "sub") == 0){ return SUB; }
	else if (strcmp(word, "and") == 0){ return AND; }
	else if (strcmp(word, "or") == 0){ return OR; }
	else if (strcmp(word, "nor") == 0){ return NOR; }
	else if (strcmp(word, "move") == 0){ return MOVE; }
	else if (strcmp(word, "mvhi") == 0){ return MVHI; }
	else if (strcmp(word, "mvlo") == 0){ return MVLO; }
	else if (strcmp(word, "addi") == 0){ return ADDI; }
	else if (strcmp(word, "subi") == 0){ return SUBI; }
	else if (strcmp(word, "andi") == 0){ return ANDI; }
	else if (strcmp(word, "ori") == 0){ return ORI; }
	else if (strcmp(word, "nori") == 0){ return NORI; }
	else if (strcmp(word, "bne") == 0){ return BNE; }
	else if (strcmp(word, "beq") == 0){ return BEQ; }
	else if (strcmp(word, "blt") == 0){ return BLT; }
	else if (strcmp(word, "bgt") == 0){ return BGT; }
	else if (strcmp(word, "lb") == 0){ return LB; }
	else if (strcmp(word, "sb") == 0){ return SB; }
	else if (strcmp(word, "lw") == 0){ return LW; }
	else if (strcmp(word, "sw") == 0){ return SW; }
	else if (strcmp(word, "lh") == 0){ return LH; }
	else if (strcmp(word, "sh") == 0){ return SH; }
	else if (strcmp(word, "jmp") == 0){ return JMP; }
	else if (strcmp(word, "la") == 0){ return LA; }
	else if (strcmp(word, "call") == 0){ return CALL; }
	else if (strcmp(word, "stop") == 0){ return STOP; }
	else if (strcmp(word, "asciz") == 0){ return ASCIZ; }
	else if (strcmp(word, "db") == 0){ return DB; }
	else if (strcmp(word, "dw") == 0){ return DW; }
	else if (strcmp(word, "dh") == 0){ return DH; }
	else if (strcmp(word, "extern") == 0){ return EXTERN; }
	else {return -1;}
    	

}


int ascizSize(char* command)
{
	char* p = command;
	int counter = 0;
	while((*p) != SPACE){
	    p++;
	}

	while((*p) == SPACE){
	    p++;
	}

	while(*p != NL){
	    p++;
	    counter++;
	}
	return counter-1;
}


int countComma(char* command)
{
	int count = 0;
	char* p = command;
	if(checkTwoPoints(p) == 1)
	{
		while( (*p) != TWOPOINTS )
		{
			p++;
		}
	}
	p++;
	while( (*p) != NL )
	{	
		if((*p) == COMMA)
		{
			count++;
		}
		p++;
	}
	return count+1;
}



pData dataTable = NULL;


void freeData()
{
	pData p;
	if(dataTable == NULL)
	{
		return;
	}
	p = dataTable;
	dataTable = dataTable->next;
	while(dataTable != NULL)
	{
		free(p);
		p = dataTable;
		dataTable = dataTable->next;
	}
	free(p);
}


void structDataTable(FILE* file, char* fileName)
{	
	int count1 = 1;
	int value = -1;
	int typeCommand = -1;
	char symbol[33] =""; 
	int count = 100;
	char command[500] = "";

	getLine(file, command);
	while(strcmp(command, "EOF") != 0)
	{
		if(strcmp(command, " \n") != 0)
		{
			getSymbol(command,symbol);
			typeCommand = type(command);
			value = count;
			if(typeCommand >= ADD && typeCommand <= STOP)
			{
				typeCommand = COMMAND;
				if( strcmp(symbol,"") != 0)
				{
					pushToData(&dataTable, symbol, value, typeCommand, count1);
				}
				count = count + 4;
			}
			if(typeCommand == EXTERN)
			{
				pushToData(&dataTable, symbol, 0, typeCommand, count1);
			}
		}
		count1++;
		getLine(file, command);
	}


	fclose(file);
	file = fopen(fileName,"r");
	count1 = 1;
	value = -1;
	typeCommand = -1;
	strcpy(symbol, "");
	getLine(file, command);
	while(strcmp(command, "EOF") != 0)
	{
		if(strcmp(command, " \n") != 0)
		{
			getSymbol(command,symbol);
			typeCommand = type(command);
			value = count;
			if(typeCommand == ASCIZ)
			{
				typeCommand = DATA;
				count = count + ascizSize(command);
				if( strcmp(symbol,"") != 0)
				{
					pushToData(&dataTable, symbol, value, typeCommand, count1);
				}
			}
			if(typeCommand == DB)
			{
				typeCommand = DATA;
				count = count + countComma(command);
				if( strcmp(symbol,"") != 0)
				{
					pushToData(&dataTable, symbol, value, typeCommand, count1);
				}
			}
			if(typeCommand == DH)
			{
				typeCommand = DATA;
				count = count + countComma(command)*2;
				if( strcmp(symbol,"") != 0)
				{
					pushToData(&dataTable, symbol, value, typeCommand, count1);
				}
			}
			if(typeCommand == DW)
			{
				typeCommand = DATA;
				count = count + countComma(command)*4;
				if( strcmp(symbol,"") != 0)
				{
					pushToData(&dataTable, symbol, value, typeCommand, count1);
				}
			}
		}
		count1++;
		getLine(file, command);
	}
	fclose(file);
	file = fopen(fileName,"r");
}


int lineNumber(char* symbol)
{
	pData p = dataTable;
	while(p != NULL)
	{
		if(strcmp(p->symbol,symbol) == 0)
		{
			return p->value;
		}
		p = p->next;
	}
	return -1;
}

int typeSymbol(char* symbol)
{
	pData p = dataTable;
	while(p != NULL)
	{
		if(strcmp(p->symbol,symbol) == 0)
		{
			return p->type;
		}
		p = p->next;
	}
	return -1;
}




void pushToData(pData* data, char* symbol, int value, int type, int count1)
{
	pData tail = *data;
	if(tail == NULL)
	{
		tail = (pData)malloc(sizeof(DataTable));
		strcpy(tail->symbol,symbol);
		tail->value = value;
		tail->type = type;
		tail->next = NULL;
		*data = tail;
		return;
	}
	while(tail->next != NULL)
	{
		if( strcmp(tail->symbol,symbol) == 0 )
		{
			if(type != EXTERN || tail->type != EXTERN)
			{
				pushToValid(&validList, "symbol already exist", count1);
			}
			return;
		}
		tail = tail->next;
	}
	if( strcmp(tail->symbol,symbol) == 0 )
	{
		if(type != EXTERN || tail->type != EXTERN)
		{
			pushToValid(&validList, "symbol already exist", count1);
		}
		return;
	}
	tail->next = (pData)malloc(sizeof(DataTable));
	tail = tail->next;
	strcpy(tail->symbol,symbol);
	tail->value = value;
	tail->type = type;
	tail->next = NULL;
}




void getSymbol(char* command, char* symbol)
{
	char* p;
	strcpy(symbol,"");
	if(checkTwoPoints(command) == 0 || type(command) == EXTERN)
	{
		if(type(command) == EXTERN)
		{
			p = command;
			while((*p) != SPACE)
			{
				p++;
			}
			while((*p) == SPACE)
			{
				p++;
			}
			while((*p) != NL)
			{
				strncat(symbol,p,1);
				p++;
			}
			return;
		}
		strcpy(symbol,"");
		return;
	}
	p = command;
	while( (*p) != TWOPOINTS )
	{
		strncat(symbol,p,1);
		p++;
	}
}


void getLine(FILE* file, char* command)
{
	char current[400] = "";	
	char* ascizPointer;
	char* current1;
	int countStart = 0;
	int countEnd = 0;
	int count = 1;
	char save;
	int end2;
	int end3;
	int start2;
	int j;
	strcpy(command, "");
	save = fgetc(file);
	if(save == NL)
	{
		strcpy(command, " \n");
		return;
	}
	if(save == EOF)
	{
		strcpy(command, "EOF");
		return;
	}
	if(save == POINTCOMMA)
	{
		while( (save = fgetc(file)) != NL)
		strcpy(command, " \n");
		return;
	}
	while(save != NL && save != POINTCOMMA) 
	{
		strncat(current,&save,1);
		save = fgetc(file);
		count++;
	}
	if(save == POINTCOMMA)
	{
		while( save != NL)
		{
			save = fgetc(file);
		}
	}
	strncat(current,&save,1);
	current1 = current;
	while( (*current1) == SPACE ) 
	{
		current1++;

	}
	if(current1[0] == POINTCOMMA)
	{
		strcpy(command, " \n");
		return;
	}
	if(checkTwoPoints(current1) == 1)
	{
		while((*current1) != TWOPOINTS)  
		{
			strncat(command,current1,1);
			current1++;
		}
		strncat(command,current1,1);
		current1++;
	}
	while( (*current1) == SPACE  || (*current1) == TAB ) 
	{
		current1++;
	}
	if(strcmp(current1,"\n") == 0 && strcmp(command,"") == 0)
	{
		strcpy(command, " \n");
		return;
	}
	while((*current1) != SPACE && (*current1) != NL && (*current1) != TAB)  
	{
		strncat(command,current1,1);
		current1++;
	}
	if((*current1) == NL)
	{
		strncat(command,current1,1);
		return;
	}
	current1++;
	ascizPointer = command;

	if(checkTwoPoints(command) == 1)
	{
		while(ascizPointer[0] != TWOPOINTS)
		{
			ascizPointer++;
		}
		ascizPointer++;
	}

	while(current1[countStart] == SPACE || current1[countStart] == TAB)
	{
		countStart++;
	}
	countEnd = countStart;
	while(!(current1[countEnd] == NL || current1[countEnd] == TE))
	{
		countEnd++;
	}
	countEnd--;
	while(current1[countEnd] == SPACE || current1[countEnd] == TAB)
	{
		countEnd--;
	}

	if(strcmp(ascizPointer, ".asciz") == 0)
	{
		int i;
		strncat(command," ",1);
		for(i = countStart ; i<= countEnd ; i++)
		{
			strncat(command,&current1[i],1);
		}
		strncat(command,"\n",1);
		return;
	}
	if(strcmp(ascizPointer,"stop") != 0){strncat(command," ",1);}
	/*printf("the command:%s     ",command);
	printf("the rest is:%s\n",current1);*/
	start2 = countStart;
	while(current1[start2] != NL) 
	{
		while(current1[start2] == SPACE || current1[start2] == TAB)
		{
			start2++;
		}
		end2 = start2;
		while(current1[end2] != COMMA && current1[end2] != NL)
		{
			end2++;
		}
		end3 = end2-1;
		while(current1[end3] == SPACE || current1[end3] == TAB)
		{
			end3--;
		}
		/*printf("start is:%c     ",current1[start2]);
		printf("end is:%c\n",current1[end3]);
		printf("end2 is:%c",current1[end2]);*/
		for(j = start2 ; j<= end3 ; j++)
		{
			strncat(command,&current1[j],1);
		}
		if(current1[end2] == NL)
		{
			break;
		}
		strncat(command,",",1);
		start2 = end2+1;	
	}
	strncat(command,"\n",1);
}



opCode opcode[27];

void structOpcodeTable(struct opcodeTable* opcode)
{
	int i;
	for(i = 0 ; i<27 ; i++)
	{
		if(i <= NOR)
		{
			opcode[i].name = i;
			opcode[i].type = 'R';
			opcode[i].funct = i+1;
			opcode[i].opcode = 0;
			continue;
		}
		if(i <= MVLO)
		{
			opcode[i].name = i;
			opcode[i].type = 'R';
			opcode[i].funct = i-4;
			opcode[i].opcode = 1;
			continue;
		}
		if(i <= SH)
		{
			opcode[i].name = i;
			opcode[i].type = 'I';
			opcode[i].funct = 0;
			opcode[i].opcode = i+2;
			continue;
		}
		if(i < STOP)
		{
			opcode[i].name = i;
			opcode[i].type = 'J';
			opcode[i].funct = 0;
			opcode[i].opcode = i+7;
			continue;
		}
		if(i == STOP)
		{
			opcode[i].name = i;
			opcode[i].type = 'J';
			opcode[i].funct = 0;
			opcode[i].opcode = 63;
			continue;
		}
		opcode[i].name = i;
		opcode[i].type = 'J';
		opcode[i].funct = 0;
		opcode[i].opcode = i+2;
	}
}



void variablesR(char* command, int function, int *arr)
{
	char* word = command;
	while(word[0] != SPACE)
	{
		word++;
	}
	word++;
	word++;
	if(function >= ADD && function <= NOR)
	{
		char word1[32] = "";
		while(word[0] != COMMA)
		{
			strncat(word1,word,1);
			word++;
		}
		arr[0] = atoi(word1);
		word++;
		word++;
		strcpy(word1, "");
		while(word[0] != COMMA)
		{
			strncat(word1,word,1);
			word++;
		}
		arr[1] = atoi(word1);
		word++;
		word++;

		strcpy(word1, "");
		while(word[0] != NL)
		{
			strncat(word1,word,1);
			word++;
		}
		arr[2] = atoi(word1);
	}
	else
	{
		char word1[32] = "";
		while(word[0] != COMMA)
		{
			strncat(word1,word,1);
			word++;
		}
		arr[0] = atoi(word1);
		word++;
		word++;

		strcpy(word1, "");
		while(word[0] != NL)
		{
			strncat(word1,word,1);
			word++;
		}
		arr[2] = atoi(word1);
		arr[1] = 0;
	}
}


int typeR(char* command, FILE* file)
{
	char binOutput[50] = "";
	char output[100] = "";
	char save[32] = "";
	int function = type(command);
	int code = opcode[function].opcode;
	int funct = opcode[function].funct;
	int arr[3]= {0,0,0};
	variablesR(command,function,arr);

	strncat(binOutput, decimalToBin(code, 6),6);
	strncat(binOutput, decimalToBin(arr[0], 5),5);
	strncat(binOutput, decimalToBin(arr[1], 5),5);
	strncat(binOutput, decimalToBin(arr[2], 5),5);
	strncat(binOutput, decimalToBin(funct, 5),5);
	strncat(binOutput, "000000", 6);

	binToHex(binOutput, 32,save);
	strncat(output,save,8);
	reverse(output);

	writeOnFile(file, output);
	return 0;
}



void variablesI(char* command, int function, int *arr,int count)
{
	char* word = command;
	while(word[0] != SPACE)
	{
		word++;
	}
	word++;
	word++;
	if( (function >= ADDI && function <= NORI) || (function >= LB && function <= SH) )
	{
		char word1[50] = "";
		
		while(word[0] != COMMA)
		{
			strncat(word1,word,1);
			word++;
		}
		arr[0] = atoi(word1);
		word++;

		strcpy(word1, "");
		while(word[0] != COMMA)
		{
			strncat(word1,word,1);
			word++;
		}
		arr[2] = atoi(word1);
		word++;
		word++;

		strcpy(word1, "");
		while(word[0] != NL)
		{
			strncat(word1,word,1);
			word++;
		}
		arr[1] = atoi(word1);
	}
	else if(function >= BNE && function<= BGT )
	{
		char word1[32] = "";
		while(word[0] != COMMA)
		{
			strncat(word1,word,1);
			word++;
		}
		arr[0] = atoi(word1);
		word++;
		word++;

		strcpy(word1, "");
		while(word[0] != COMMA)
		{
			strncat(word1,word,1);
			word++;
		}
		word++;
		arr[1] = atoi(word1);
		strcpy(word1,"");
		while(word[0] != NL)
		{
			strncat(word1,word,1);
			word++;
		}
		arr[2] = lineNumber(word1)-count;
	}
}



int typeI(char* command, int count, FILE* file)
{
	char binOutput[200] = "";
	char output[100] = "";
	char save[8] = "";
	int function = type(command);
	int code = opcode[function].opcode;
	int arr[3]= {0,0,0};
	variablesI(command,function,arr,count);

	strncat(binOutput, decimalToBin(code, 6),6);
	strncat(binOutput, decimalToBin(arr[0], 5),5);
	strncat(binOutput, decimalToBin(arr[1], 5),5);
	strncat(binOutput, decimalToBin2n(arr[2], 16),16);

	binToHex(binOutput, 32,save);
	strncat(output, save, 8);
	reverse(output);

	writeOnFile(file, output);
	return 0;
}


void variablesJ(char* command, int function, int *arr,int count)
{
	if(function == JMP)
	{
		char* word = command;
		char word1[50] = ""; 
		while(word[0] != SPACE)
		{
			word++;
		}
		word++;
		if(word[0] == '$')
		{
			arr[0] = 1;
			word++;
			while(word[0] != NL)
			{
				strncat(word1,word,1);
				word++;
			}
			arr[1] = atoi(word1);
		}
		else
		{
			arr[0] = 0;
			while(word[0] != NL)
			{
				strncat(word1,word,1);
				word++;
			}
			arr[1] = lineNumber(word1);
		}
	}
	else if(function == LA || function == CALL)
	{
		char* word = command;
		char word1[50] = ""; 
		while(word[0] != SPACE)
		{
			word++;
		}
		word++;
		arr[0] = 0;
		while(word[0] != NL)
		{
			strncat(word1,word,1);
			word++;
		}
		arr[1] = lineNumber(word1);
	}
	else if(function == STOP)
	{
		arr[0] = 0;
		arr[1] = 0;
	}
	

}



int typeJ(char* command, int count, FILE* file)
{
	char save[32] = "";
	char binOutput[200] = "";
	char output[100] = "";
	int function = type(command);
	int code = opcode[function].opcode;
	int arr[2]= {0,0};
	variablesJ(command,function,arr,count);
	strncat(binOutput, decimalToBin(code, 6),6);
	strncat(binOutput, decimalToBin(arr[0], 1),1);
	strncat(binOutput, decimalToBin(arr[1], 25),25);
	binToHex(binOutput, 32,save);
	strncat(output, save, 8);
	reverse(output);

	writeOnFile(file, output);
	return 0;
}



void typeAsciz(char* command, FILE* file)
{
	char* word = command;
	char binOutput[800] = "";
	char output[200] = "";
	int saveChar = 0;
	char save[8] = "";
	while(word[0] != SPACE)
	{
		word++;
	}
	word += 2;
	saveChar = word[0];
	word++;
	while(!(word[0] == NL && saveChar == QUOT))
	{
		strncat(binOutput,decimalToBin(saveChar,8),8);
		strcpy(save,"");
		binToHex(binOutput, 8,save);
		strncat(output, save, 2);
		saveChar = word[0];
		word++;
		strcpy(binOutput, "");
	}
	strncat(output,"00",2); 

	writeOnFile(file, output);
}



void typeDb(char* command, FILE* file)
{
	char binOutput[300] = "";
	char* word = command;
	char output[300] = "";
	char number[10] = "";
	char save[8] = "";
	while(word[0] != SPACE)
	{
		word++;
	}
	word++;
	while(word[0] != NL)
	{
		while(word[0] != COMMA && word[0] != NL)
		{
			strncat(number,word,1);
			word++;
		}
		strncat(binOutput,decimalToBin2n(atoi(number),8),8);
		strcpy(save,"");
		binToHex(binOutput, 8,save);
		strncat(output, save , 2);
		if(word[0] == NL)
		{
			break;
		}
		word++;
		strcpy(number,"");
		strcpy(binOutput,"");
	}
	writeOnFile(file, output);
}




void typeDh(char* command, FILE* file)
{
	char binOutput[300] = "";
	char* word = command;
	char output[300] = "";
	char save[100] = "";
	char number[10] = "";
	char save1[16] = "";
	while(word[0] != SPACE)
	{
		word++;
	}
	word ++;
	while(word[0] != NL)
	{
		while(word[0] != COMMA && word[0] != NL)
		{
			strncat(number,word,1);
			word++;
		}
		strcpy(binOutput,decimalToBin2n(atoi(number),16));
		strcpy(save1,"");
		binToHex(binOutput, 16,save1);
		strcpy(save, save1);
		reverse(save);
		strncat(output,save,4);
		if(word[0] == NL)
		{
			break;
		}
		word++;
		strcpy(number,"");
		strcpy(binOutput,"");
	}
	writeOnFile(file, output);
}



void typeDw(char* command, FILE* file)
{
	char binOutput[300] = "";
	char* word = command;
	char save1[32] = "";
	char output[300] = "";
	char save [100] = "";
	char number[10] = "";
	while(word[0] != SPACE)
	{
		word++;
	}
	word ++;
	while(word[0] != NL)
	{
		while(word[0] != COMMA && word[0] != NL)
		{
			strncat(number,word,1);
			word++;
		}
		strcpy(binOutput,decimalToBin2n(atoi(number),32));
		strcpy(save1,"");
		binToHex(binOutput, 32,save1);
		strcpy(save, save1);
		reverse(save);
		strncat(output,save,8);
		if(word[0] == NL)
		{
			break;
		}
		word++;
		strcpy(number,"");
		strcpy(binOutput,"");
		strcpy(save,"");
	}
	writeOnFile(file, output);
}




int checkComma(char* command)
{
	char* p = command;
	while(p[0] != NL)
	{
		if(p[0] == COMMA){return 1;}
		p++;
	}
	return 0;
}


int stringLength(char* command)
{
	int count = 0;
	char* p = command;
	while(p[0] != NL)
	{
		count++;
		p++;
	}
	return count;
}


int alphaBet(char a)
{
	if( (a>=65 && a<=90) || (a>=97 && a<= 122) )
	{
		return 1;
	}
	return 0;
}


int validSymbol(char* symbol)
{
	char* p = symbol;
	if( strlen(symbol) < 7 )
	{
		if( type(symbol) != -1)
		{
			return 0;
		}
	}
	if( alphaBet(*p) == 0 ){return 0;}
	p++;
	while((*p) != NL && (*p) != TE)
	{
		if( alphaBet(*p) == 0 && ( (*p) <= 48 || (*p) >= 57 ) )
		{
			return 0;
		}
		p++;
	}
	return 1;
}



int inRange16(char* number)
{
	int a = atoi(number);
	if(a >= -32768 && a <= 32767)
	{
		return 1;
	}
	return 0;
}

int inRange8(char* number)
{
	int a = atoi(number);
	if(a >= -128 && a <= 127)
	{
		return 1;
	}
	return 0;
}

int inRange32(char* number1)
{
	int64_t min = -2147483647-1;
	int a ;
	char number[200] = "";
	char numberIn[200] = "";
	strcpy(number,number1);
	if(number[0] == '+')
	{
		int i = 1;
		for(;i<200;i++)
		{
			number[i-1] = number[i];
		}
		number[200] = TE;
	}
	a = atoi(number);
	sprintf(numberIn, "%d", a);
	if( strcmp(number, numberIn) != 0)
	{
		return 0;
	}
	if(a >= min && a <= 2147483647)
	{
		return 1;
	}
	return 0;
}

int inRange(char* number)
{
	int a = atoi(number);
	if(number[0] == '+' || number[0] == '-')
	{
		return 0;
	}
	if(a >= 0 && a <= 31)
	{
		return 1;
	}
	return 0;
}


void getNumber(char** p , char* number)
{
	strcpy(number, "");
	while((*p)[0] != COMMA && (*p)[0] != NL)
	{
		strncat(number, (*p), 1);
		(*p)++;
	}
}


int doublePsik(char* command)
{
	int i;
	for(i=0 ; i<strlen(command)-1 ; i++)
	{
		if(command[i] == ',' && command[i+1] == ',')
		{
			return 1;
		}
	}
	return 0;
}


int checkNumber(char* a)
{
	int count = 0;
	if(a[0] == '-' || a[0] == '+'){count++;}
	while(a[count] != '\0' && isdigit(a[count]) != 0)
	{
		count++;
	}
	if(a[count] != '\0')
	{
		return 0;
	}
	return 1;
}


int isValid(char* command)
{
	char number[50] = "";
	char* p = command;
	int typeCommand = type(command);
	char checkLabel[81] = "";
	if(strcmp(command, " \n") == 0){return CORRECT;}

	if(doublePsik(command) == 1){return DOUBLECOMMA;}

	if(stringLength(command) > 80){return THEMAXIS80;}
	
	if( checkTwoPoints(p) == 1 )
	{
		while(p[0] != TWOPOINTS)
		{
			strncat(checkLabel, p, 1);
			p++;
		}
		strncat(checkLabel, "\n", 1);
		if(stringLength(checkLabel) > 32)
		{
			return SYMBOLTOOLONG;
		}
		if(validSymbol(checkLabel) == 0)
		{
			return NOTSYMBOL;
		}
		p++;
		if(strcmp(p, "\n") == 0)
		{
			return EMPTYSYMBOL;
		}
	}

	if(typeCommand == -1)
	{
		return FUNCTIONERROR; 
	}

	while((*p) != SPACE && (*p) != TAB && (*p) != NL)
	{
		p++;
	}
	if(typeCommand >= ADD && typeCommand <= NOR)
	{
		if(p[0] == NL){return NEEDMORE;}
		p++;
		if( p[0] == DOLLAR )
		{
			p++;
			if(p[0] == NL){return NEEDMORE;}
			getNumber(&p, number);
			if(p[0] == NL){return NEEDMORE;}
			p++;
			if(p[0] == NL){return NEEDMORE;}
			if(checkNumber(number) == 1 && inRange(number) == 1)
			{
				if( p[0] == DOLLAR )
				{
					p++;
					if(p[0] == NL){return NEEDMORE;}
					getNumber(&p, number);
					if(p[0] == NL){return NEEDMORE;}
					p++;
					if(p[0] == NL){return NEEDMORE;}
					if(checkNumber(number) == 1 && inRange(number) == 1)
					{
						if( p[0] == DOLLAR )
						{
							p++;
							if(p[0] == NL){return NEEDMORE;}
							getNumber(&p, number);
							if(checkNumber(number) == 1 && inRange(number) == 1)
							{
								if(p[0] == NL){return CORRECT;}
								else{return TOOMUCH;}
							}
							else{if(checkNumber(number) == 0){return NOTNUMBER;} else {return OUTOFRANGE;}}
						}
						else{return TYPEOPERAND;}
					}
					else{if(checkNumber(number) == 0){return NOTNUMBER;} else {return OUTOFRANGE;}}
				}
				else{return TYPEOPERAND;}
			}
			else{if(checkNumber(number) == 0){return NOTNUMBER;} else {return OUTOFRANGE;}}
		}
		else{return TYPEOPERAND;}
	}

	else if(typeCommand >= MOVE && typeCommand <= MVLO)
	{
		if(p[0] == NL){return NEEDMORE;}
		p++;
		if( p[0] == DOLLAR )
		{
			p++;
			if(p[0] == NL){return NEEDMORE;}
			getNumber(&p, number);
			if(p[0] == NL){return NEEDMORE;}
			p++;
			if(p[0] == NL){return NEEDMORE;}
			if(checkNumber(number) == 1 && inRange(number) == 1)
			{
				if( p[0] == DOLLAR )
				{
					p++;
					if(p[0] == NL){return NEEDMORE;}
					getNumber(&p, number);
					if(checkNumber(number) == 1 && inRange(number) == 1)
					{
						if(p[0] == NL){return CORRECT;}
						else{return TOOMUCH;}
					}
					else
					{
						if(checkNumber(number) == 0){return NOTNUMBER;} 
						else {return OUTOFRANGE;}
					}
				}
				else{return TYPEOPERAND;}
			}
			else
			{
				if(checkNumber(number) == 0){return NOTNUMBER;} 
				else {return OUTOFRANGE;}
			}
		}
		else{return TYPEOPERAND;}
	}
	
	else if(  (typeCommand >= ADDI && typeCommand <= NORI)  ||  (typeCommand >= LB && typeCommand <= SH)  )
	{
		if(p[0] == NL){return NEEDMORE;}
		p++;
		if( p[0] == DOLLAR )
		{
			p++;
			if(p[0] == NL){return NEEDMORE;}
			getNumber(&p, number);
			if(p[0] == NL){return NEEDMORE;}
			p++;
			if(p[0] == NL){return NEEDMORE;}
			if(checkNumber(number) == 1 && inRange(number) == 1)
			{
				if(p[0] != DOLLAR)
				{
					getNumber(&p, number);
					if(p[0] == NL){return NEEDMORE;}
					p++;
					if(p[0] == NL){return NEEDMORE;}
					if(checkNumber(number) == 1 && inRange16(number) == 1)
					{
						if( p[0] == DOLLAR )
						{
							p++;
							if(p[0] == NL){return NEEDMORE;}
							getNumber(&p, number);
							if(checkNumber(number) == 1 && inRange(number) == 1)
							{
								if(p[0] == NL){return CORRECT;}
								else{return TOOMUCH;}
							}
							else{if(checkNumber(number) == 0){return NOTNUMBER;} else {return OUTOFRANGE;}}
						}
						else{return TYPEOPERAND;}
					}
					else{return IMMERROR;}
				}
				else{return TYPEOPERAND;}
			}
			else{if(checkNumber(number) == 0){return NOTNUMBER;} else {return OUTOFRANGE;}}
		}
		else{return TYPEOPERAND;}
	}

	else if(typeCommand >= BNE && typeCommand <= BGT) 
	{
		if(p[0] == NL){return NEEDMORE;}
		p++;
		if( p[0] == DOLLAR )
		{
			p++;
			if(p[0] == NL){return NEEDMORE;}
			getNumber(&p, number);
			if(p[0] == NL){return NEEDMORE;}
			p++;
			if(p[0] == NL){return NEEDMORE;}
			if(checkNumber(number) == 1 && inRange(number) == 1)
			{
				if( p[0] == DOLLAR )
				{
					p++;
					if(p[0] == NL){return NEEDMORE;}
					getNumber(&p, number);
					if(p[0] == NL){return NEEDMORE;}
					p++;
					if(p[0] == NL){return NEEDMORE;}
					if(checkNumber(number) == 1 && inRange(number) == 1)
					{
						if(checkComma(p) == 1){return TOOMUCH;}
						if(validSymbol(p) == 1)
						{
							char label[33] = "";					
							while(p[0] != NL)
							{
								strncat(label, p, 1);
								p++;
							}
							if(lineNumber(label) == -1){return SYMBOLNOTEXIST;}
							if(typeSymbol(label) == EXTERN){return ERROR;}
							return CORRECT;
						}
						else{return NOTSYMBOL;}
					}
					else{if(checkNumber(number) == 0){return NOTNUMBER;} else {return OUTOFRANGE;}}
				}
				else{return TYPEOPERAND;}
			}
			else{if(checkNumber(number) == 0){return NOTNUMBER;} else {return OUTOFRANGE;}}
		}
		else{return TYPEOPERAND;}
	}

	else if(typeCommand == JMP)
	{
		if(p[0] == NL){return NEEDMORE;}
		p++;
		if(p[0] == NL){return NEEDMORE;}
		if( p[0] == DOLLAR )
		{
			p++;
			if(p[0] == NL){return NEEDMORE;}
			getNumber(&p, number);
			if( checkNumber(number) == 1 && inRange(number) == 1 )
			{
				if(p[0] == NL){return CORRECT;}
				else{return TOOMUCH;}
			}
			else
			{
				if(checkNumber(number) == 0) {return NOTNUMBER;}
				else {return OUTOFRANGE;}
			}
		}
		if(checkComma(p) == 1)
		{
			return TOOMUCH;
		}
		if( validSymbol(p) == 1 )
		{
			char label[33] = "";					
			while(p[0] != NL)
			{
				strncat(label, p, 1);
				p++;
			}
			if(lineNumber(label) == -1){return SYMBOLNOTEXIST;}
			return CORRECT;
		}
		return NOTSYMBOL;
	}

	else if(typeCommand == LA || typeCommand == CALL || typeCommand == EXTERN)
	{
		if(p[0] == NL){return NEEDMORE;}
		p++;
		if(checkComma(p) == 1){return TOOMUCH;}
		if( validSymbol(p) == 1 )
		{
			if(typeCommand == LA || typeCommand == CALL)
			{
				char label[33] = "";					
				while(p[0] != NL)
				{
					strncat(label, p, 1);
					p++;
				}
				if(lineNumber(label) == -1){return SYMBOLNOTEXIST;}
				return CORRECT;
			}
			return CORRECT;
		}
		return NOTSYMBOL;
	}

	else if(typeCommand == STOP )
	{
		if( p[0] == NL )
		{
			return CORRECT;
		}
		return ERROR;
	}

	else if(typeCommand == ASCIZ)
	{
		char saveLastChar;
		int counterChar = 0;
		if(p[0] == NL){return NEEDMORE;}
		p++;
		if(p[0] == NL){return NEEDMORE;}
		if(p[0] == QUOT)
		{
			p++;
			if(p[0] == NL){return NEEDMORE;}
			while( p[0] != NL )
			{
				saveLastChar = p[0];
				counterChar++;
				p++;
			}
			if( counterChar > 80 ){return THEMAXIS12;}
			if(saveLastChar == QUOT)
			{
				if(p[0] == NL){return CORRECT;}
				else{return TOOMUCH;}
			}
			else{return NOTQUOT;}
		}
		else{return NOTQUOT;}
	}
	
	else if(typeCommand == DB)
	{
		int last = 0;
		while(p[last] != NL)
		{
			last++;
		}
		last--;
		while(p[last] == SPACE || p[last] == TAB)
		{
			last--;
		}
		if(p[last] == COMMA || p[1] == COMMA){return ERROR;}
		if(p[0] == NL){return NEEDMORE;}
		p++;
		while(p[0] != NL)
		{
			getNumber(&p, number);
			if(checkNumber(number) == 0){return NOTNUMBER;}
			if(inRange8(number) == 0){return OUTOFRANGE;}
			if(p[0] != COMMA && p[0] != NL){return MISSCOMMA;}
			if(p[0] == COMMA){p++;}
		}
		return CORRECT;
	}

	else if(typeCommand == DH)
	{
		int last = 0;
		while(p[last] != NL)
		{
			last++;
		}
		last--;
		while(p[last] == SPACE || p[last] == TAB)
		{
			last--;
		}
		if(p[last] == COMMA || p[1] == COMMA){return ERROR;}
		if(p[0] == NL){return NEEDMORE;}
		p++;
		while(p[0] != NL)
		{
			getNumber(&p, number);
			if(checkNumber(number) == 0){return NOTNUMBER;}
			if(inRange16(number) == 0){return OUTOFRANGE;}
			if(p[0] != COMMA && p[0] != NL){return MISSCOMMA;}
			if(p[0] == COMMA){p++;}
		}
		return CORRECT;
	}

	else if(typeCommand == DW)
	{
		int last = 0;
		while(p[last] != NL)
		{
			last++;
		}
		last--;
		while(p[last] == SPACE || p[last] == TAB)
		{
			last--;
		}
		if(p[last] == COMMA || p[1] == COMMA){return ERROR;}
		if(p[0] == NL){return NEEDMORE;}
		p++;
		while(p[0] != NL)
		{
			getNumber(&p, number);
			if(checkNumber(number) == 0){return NOTNUMBER;}
			if(inRange32(number) == 0){return OUTOFRANGE;}
			if(p[0] != COMMA && p[0] != NL){return MISSCOMMA;}
			if(p[0] == COMMA){p++;}
		}
		return CORRECT;
	}
	else if(typeCommand == ENTRY)
	{
		char label[33] = "";
		if(p[0] == NL){return NEEDMORE;}
		p++;
		if(checkComma(p) == 1){return TOOMUCH;}
		if( validSymbol(p) == 1 )
		{
			while(p[0] != NL)
			{
				strncat(label, p, 1);
				p++;
			}
			
			if(lineNumber(label) == -1)
			{
				return SYMBOLNOTEXIST;
			}
			if(typeSymbol(label) == EXTERN)
			{
				return ERROR;
			}
			return CORRECT;
		}
		return NOTSYMBOL;
	}

	return 1;
	
}





void structValidList(FILE* file, char* fileName)
{
	int errorValue = 0;
	int count = 1;
	char command[500];
	getLine(file, command);
	while( strcmp(command,"EOF") != 0 )
	{	
		errorValue = isValid(command);
		if(errorValue != 1)
		{
			if(errorValue == 2){pushToValid(&validList, "function dont exist", count);}
			else if(errorValue == 3){pushToValid(&validList, "type error input", count);}
			else if(errorValue == 4){pushToValid(&validList, "not number input", count);}
			else if(errorValue == 5){pushToValid(&validList, "too much input", count);}
			else if(errorValue == 6){pushToValid(&validList, "out of range", count);}
			else if(errorValue == 7){pushToValid(&validList, "need more input", count);}
			else if(errorValue == 8){pushToValid(&validList, "error with immed", count);}
			else if(errorValue == 9){pushToValid(&validList, "symbol not valid", count);}
			else if(errorValue == 10){pushToValid(&validList, "error", count);}
			else if(errorValue == 11){pushToValid(&validList, "missing quot", count);}
			else if(errorValue == 12){pushToValid(&validList, "double comma", count);}
			else if(errorValue == 13){pushToValid(&validList, "the max length command is 80", count);}
			else if(errorValue == 14){pushToValid(&validList, "the max length symbol is 32", count);}
			else if(errorValue == 15){pushToValid(&validList, "the max length string is 12", count);}
			else if(errorValue == 16){pushToValid(&validList, "the symbol dont exist", count);}
			else if(errorValue == 18){pushToValid(&validList, "symbol with empty command", count);}
		}
		count++;
		getLine(file, command);
	}
	fclose(file);
	file = fopen(fileName,"r");
}



void pushToValid(pValid* valid, char* error, int value)
{
	pValid tail = *valid;
	if(tail == NULL)
	{
		tail = (pValid)malloc(sizeof(ValidList));
		strcpy(tail->error,error);
		tail->value = value;
		tail->next = NULL;
		*valid = tail;
		return;
	}
	while(tail->next != NULL)
	{
		tail = tail->next;
	}
	tail->next = (pValid)malloc(sizeof(ValidList));
	tail = tail->next;
	strcpy(tail->error,error);
	tail->value = value;
	tail->next = NULL;
}



int lineCounter = 0;
int writeLine = 100;
void writeOnFile(FILE* file, char* input)
{
	char* p = input;
	while(p[0] != TE)
	{
		if(lineCounter%8 ==0)
		{
			fputc(NL, file);
			writeNumber4(file, writeLine);
			writeLine += 4;
			lineCounter = 0;
		}
		if(lineCounter%2 ==0)
		{
			fputc(SPACE, file);
		}
		fputc(p[0], file);
		lineCounter++;
		p++;
	}
	
}

void writeNumber4(FILE* file, int input)
{
	char number[10] = "";
	sprintf(number, "%d", input);
	if(input < 1000)
	{
		fputc('0', file);
		fputc(number[0], file);
		fputc(number[1], file);
		fputc(number[2], file);
	}
	else
	{
		fputc(number[0], file);
		fputc(number[1], file);
		fputc(number[2], file);
		fputc(number[3], file);
	}
}

void writeNumber(FILE* file, int input)
{
	int i;
	char number[10] = "";
	sprintf(number, "%d", input);
	for(i = 0 ; i < 10 ; i++)
	{
		if(number[i] != '\0')
		{
			fputc(number[i], file);
		}
	}
	fputc(SPACE, file);
}



void writeOnEntry(FILE* file, char* input)
{
	char* p = input;
	while(p[0] != TE)
	{
		fputc(p[0], file);
		p++;
	}
	fputc(NL, file);
}


void run(char* fileName)
{
	pValid t;
	FILE* file;
	int countValid = 1;
	char finish[100] =  "";
	int start = 0;

	while( fileName[start] != '.' && fileName[start] != '\0')
	{
		start++;
	}
	if(fileName[start] == '.')
	{
		start++;
		while(fileName[start] != '\0')
		{
			strncat(finish,&fileName[start],1);
			start++;
		}
	}	

	printf("--------------------------------------------\n");
	printf("%s:\n",fileName);
	if(strcmp(finish,"as") != 0)
	{
		printf("not assembly File!\n");
		printf("--------------------------------------------\n");
		return;
	}

	structOpcodeTable(opcode);
	
	file = fopen(fileName,"r");
	if(file == NULL)
	{
		printf("file dont exist\n");
		printf("--------------------------------------------\n");
		return;
	}
	structDataTable(file,fileName);
	structValidList(file, fileName);
	t = validList;
	if(t != NULL)
	{
		while(t != NULL)
		{
			printf("%d ",countValid);
			printf("error:%s   ",t->error);
			printf("value:%d\n",t->value);
			t = t->next;
			countValid++;
		}
	}
	else
	{
		printf("SUCCESS\n");
		transform(file, fileName);
	}
	printf("--------------------------------------------\n");
	freeData();
	freeValid();
	lineCounter = 0;
	writeLine = 100;
}




