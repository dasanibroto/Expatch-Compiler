#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include "uthash.h"
#include <leveldb.h>

struct dictionary {
	char variable[256][256];        /* we'll use this field as the key */
	char value[256];
	UT_hash_handle hh; /* makes this structure hashable */
};

struct dictionary *variables;

void add_var(char* variable, char *value) {
	struct dictionary *s;
	HASH_FIND_INT(variables, &variable, s);  /* id already in the hash? */
	if (s == NULL) {
		s = (struct dictionary *)malloc(sizeof *s);
		*s->variable = variable;
		HASH_ADD_INT(variables, variable, s);
	}
	strcpy(s->value, value);
}



struct dictionary *find_var(char* variable) {
	struct dictionary *s;
	HASH_FIND_INT(variables, &variable, s);
	return s;
}

void remove_spaces(char* s) {
	const char* d = s;
	int char_trip = 0;
	do {
		if (*d != ' ') {
			char_trip = 1;
		}
		if (char_trip == 0) {
			while (*d == ' ') {
				++d;
			}
		}
	} while (*s++ = *d++);
}

char nullarr[256] = { '\0' };
char line[256] = { '\0' };
char CRC[256] = { '\0' };
char operand[2] = { '\0' };
char linecopy[256] = { '\0' };
char subbuff[256] = { '\0' };
char working_str[256] = { '\0' };
char keys[] = "=(:";
char comma[] = ",";
char unwanted[] = " ,*():";
char write_arr[256][256] = { '\0' };
int error_detect = 0;
int write_pos = 0;
char address[256];
char value[256];
int operand_pos;
char written[256] = { '\0' };
int has_written = 0;

void getwriteadd() {
	memcpy(address, &nullarr[0], 256);
	memcpy(address, &linecopy[operand_pos + 1], strcspn(linecopy, comma) - (operand_pos + 1));
}

void getwriteval() {
	memcpy(value, &nullarr[0], 256);
	memcpy(value, &linecopy[strcspn(linecopy,comma)], strlen(linecopy)- strcspn(linecopy, comma));
}

void getvarval() {
	memcpy(value, &nullarr[0], 256);
	memcpy(value, &linecopy[strcspn(linecopy, operand)+1], strlen(linecopy) - strcspn(linecopy, operand)-1);
	strtok(value, "\n");
}

void write8bit(char *address, char *value) {
	strcat(written, "patch=1,EE,0");
	strcat(written, address);
	strcat(written, ",extended,000000");
	strcat(written, value);
	strcat(written, "\n");
	strcpy(write_arr[write_pos], written);
	has_written = 1;
	write_pos++;
}

void write16bit(char *address, char *value) {
	strcat(written, "patch=1,EE,1");
	strcat(written, address);
	strcat(written, ",extended,0000");
	strcat(written, value);
	strcat(written, "\n");
	strcpy(write_arr[write_pos], written);
	has_written = 1;
	write_pos++;
}

void write32bit(char *address, char *value) {
	strcat(written, "patch=1,EE,2");
	strcat(written, address);
	strcat(written, ",extended,");
	strcat(written, value);
	strcat(written, "\n");
	strcpy(write_arr[write_pos], written);
	has_written = 1;
	write_pos++;
}

void checkwrite() {
	if (strcmp(subbuff, "write8bit") == 0) {
		getwriteadd();
		getwriteval();
		write8bit(address, value);
	}
	else if (strcmp(subbuff, "write16bit") == 0) {
		getwriteadd();
		getwriteval();
		write16bit(address, value);
	}
	else if (strcmp(subbuff, "write32bit") == 0) {
		getwriteadd();
		getwriteval();
		write32bit(address, value);
	}
}

int main(int argc, char* argv[])
{
	char const* const fileName = argv[1]; /* should check that argc > 1 */
	FILE* file = fopen(fileName, "r"); /* should check the result */


	while (fgets(line, sizeof(line), file)) {
		has_written = 0;
		memcpy(written, &nullarr[0], 256);
		memcpy(linecopy, &line[0], sizeof(line));
		remove_spaces(linecopy);
		operand_pos = strcspn(linecopy, keys);
		memcpy(subbuff, &linecopy[0], operand_pos);
		memcpy(operand, &linecopy[operand_pos], 1);
		subbuff[operand_pos] = '\0';
		operand[1] = '\0';
		memcpy(working_str, &linecopy[0], 2);
		if (strcmp(working_str, "//") == 0) {
			memcpy(subbuff, &nullarr[0], 256);
			memcpy(subbuff, &linecopy[2], operand_pos-2);
			strcat(written, "//");
			checkwrite();
			if (has_written == 0) {
				memcpy(subbuff, &linecopy[2], 254);
				strcat(written, subbuff);
				strcpy(write_arr[write_pos], written);
				write_pos++;
			}
		}
		else if (strcmp(operand, "=") == 0) {
			if (strcmp(subbuff, "CRC") == 0) {
				memcpy(CRC, &linecopy[operand_pos + 1], sizeof(linecopy) - operand_pos);
				CRC[sizeof(linecopy) - operand_pos] = '\0';
				strtok(CRC, "\n");
			}
			else {
				getvarval();
				add_var(subbuff, value);
			}
		}
		else if (strcmp(operand, "(") == 0) {
			checkwrite();
		}
		else {
			strcpy(write_arr[write_pos], "\n");
			write_pos++;
		}
	}
	fclose(file);
	FILE *fpw;
	fpw = fopen(strcat(CRC, ".expatch"), "w");
	for (int x = 0; x < (sizeof write_arr / sizeof write_arr[0]); ++x) {
		memcpy(working_str, &write_arr[x][0], 256);
		fputs(working_str, fpw);
	}
}

