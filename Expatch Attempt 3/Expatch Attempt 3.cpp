#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include "uthash.h"

struct dictionary {
	char varname[16];             /* key (string is WITHIN the structure) */
	int varstore;
	UT_hash_handle hh;         /* makes this structure hashable */
};

struct dictionary *s, *tmp, *variables = NULL;
int var_store_len = 0;
char varval[256] = { '\0' };
char var_arr[256][256] = { '\0' };
char lookup[256] = { '\0' };
char value_store[256] = { '\0' };
char nullarr[256] = { '\0' };
char line[256] = { '\0' };
char CRC[256] = { '\0' };
char operand[2] = { '\0' };
char linecopy[256] = { '\0' };
char subbuff[256] = { '\0' };
char working_str[256] = { '\0' };
char str1[256] = { '\0' };
char str2[256] = { '\0' };
char * brokenstr = { '\0' };
char keys[] = "=(:";
char comma[] = ",";
char unwanted[] = " ,*():";
char open_brk[] = "(";
char close_brk[] = ")";
char write_arr[256][256] = { '\0' };
int error_detect = 0;
int write_pos = 0;
char address[256];
char value[256];
int operand_pos;
char written[256] = { '\0' };
int has_written = 0;
int sign = 0;
int comment = 0;
int line_count1 = 9999;
int line_count2 = 9999;
int line_count3 = 9999;
int line_count4 = 9999;
int line_count5 = 9999;
int ifpos1 = 0;
int ifpos2 = 0;
int ifaddrpos1 = 0;
int ifaddrpos2 = 0;
int timerpos = 0;
int ifcounting1 = 0;
int ifcounting2 = 0;
int ifaddrcounting1 = 0;
int ifaddrcounting2 = 0;
int timercounting = 0;
char ifline1[256] = { '\0' };
char ifline2[256] = { '\0' };
char ifaddrline1[256] = { '\0' };
char ifaddrline2[256] = { '\0' };
char timerline[256] = { '\0' };

void addvar(char *name) {
	s = (struct dictionary *)malloc(sizeof *s);
	strcpy(s->varname, name);
	s->varstore = var_store_len;
	HASH_ADD_STR(variables, varname, s);
}

void getvar(char *name) {
	HASH_FIND_STR(variables, name, s);
	if (s) memcpy(varval, &var_arr[s->varstore], 256);
}

void replacevar(char *name) {
	memcpy(varval, &nullarr[0], 256);
	getvar(name);
	if (strcmp(varval, nullarr) != 0) {
		memcpy(name, &varval[0], 256);
	}
}

void incrementcounters() {
	if (timercounting == 1) {
		line_count1++;
	}
	else {
		line_count1 = 9999;
	}
	if (ifcounting1 == 1) {
		line_count2++;
	}
	else {
		line_count2 = 9999;
	}
	if (ifcounting2 == 1) {
		line_count3++;
	}
	else {
		line_count3 = 9999;
	}
	if (ifaddrcounting1 == 1) {
		line_count4++;
	}
	else {
		line_count4 = 9999;
	}
	if (ifaddrcounting2 == 1) {
		line_count5++;
	}
	else {
		line_count5 = 9999;
	}
}

void remove_spaces(char* s) {
	const char* d = s;
	int char_trip = 0;
	do {
		if ((*d != ' ')&&(*d != '	')) {
			char_trip = 1;
		}
		if (char_trip == 0) {
			while ((*d == ' ')||(*d == '	')) {
				++d;
			}
		}
	} while (*s++ = *d++);
}

void getvarval() {
	memcpy(value, &nullarr[0], 256);
	memcpy(value, &linecopy[strcspn(linecopy, operand)+1], strlen(linecopy) - strcspn(linecopy, operand)-1);
	strtok(value, "\n");
}

void get1str() {
	memcpy(value, &nullarr[0], 256);
	memcpy(working_str, &linecopy[strcspn(linecopy, open_brk) + 1], strcspn(linecopy, close_brk) - strcspn(linecopy, open_brk) - 1);
	memcpy(value, working_str, 256);
	replacevar(value);
}

void get2str() {
	memcpy(address, &nullarr[0], 256);
	memcpy(value, &nullarr[0], 256);
	memcpy(working_str, &linecopy[strcspn(linecopy, open_brk) + 1], strcspn(linecopy, close_brk) - strcspn(linecopy, open_brk) - 1);
	brokenstr = strtok(working_str, comma);
	memcpy(address, brokenstr, 256);
	brokenstr = strtok(NULL, comma);
	memcpy(value, brokenstr, 256);
	replacevar(address);
	replacevar(value);
}

void get3str() {
	memcpy(address, &nullarr[0], 256);
	memcpy(str1, &nullarr[0], 256);
	memcpy(value, &nullarr[0], 256);
	memcpy(working_str, &linecopy[strcspn(linecopy, open_brk) + 1], strcspn(linecopy, close_brk) - strcspn(linecopy, open_brk) - 1);
	brokenstr = strtok(working_str, comma);
	memcpy(address, brokenstr, 256);
	brokenstr = strtok(NULL, comma);
	memcpy(str1, brokenstr, 256);
	brokenstr = strtok(NULL, comma);
	memcpy(value, brokenstr, 256);
	replacevar(address);
	replacevar(str1);
	replacevar(value);
}

void get4str() {
	memcpy(address, &nullarr[0], 256);
	memcpy(str1, &nullarr[0], 256);
	memcpy(str2, &nullarr[0], 256);
	memcpy(value, &nullarr[0], 256);
	memcpy(working_str, &linecopy[strcspn(linecopy, open_brk) + 1], strcspn(linecopy, close_brk)-strcspn(linecopy,open_brk)-1);
	brokenstr = strtok(working_str, comma);
	memcpy(address, brokenstr, 256);
	brokenstr = strtok(NULL, comma);
	memcpy(str1, brokenstr, 256);
	brokenstr = strtok(NULL, comma);
	memcpy(str2, brokenstr, 256);
	brokenstr = strtok(NULL, comma);
	memcpy(value, brokenstr, 256);
	replacevar(address);
	replacevar(str1);
	replacevar(str2);
	replacevar(value);
}

void addzero(char *value, int len) {
	int i = 0;
	int zeroes = len - strlen(value);
	for (i = 0; i < zeroes; i++) {
		strcat(written, "0");
	}
}

void checkvalsign(char* value) {
	memcpy(working_str, &nullarr[0], 256);
	memcpy(working_str, &value[0], 1);
	if (strcmp(working_str, "-") == 0) {
		sign = 1;
		memcpy(working_str, &nullarr[0], 256);
		memcpy(working_str, &value[1], strlen(value) - 1);
		memcpy(value, &nullarr[0], 256);
		memcpy(value, &working_str[0], 256);
	}
	else {
		sign = 0;
	}
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

void change8bit(char *address, char *value) {
	checkvalsign(value);
	switch (sign) {
	case 0:
		strcat(written, "patch=1,EE,300000");
		break;
	case 1:
		strcat(written, "patch=1,EE,301000");
		break;
	}
	strcat(written, value);
	strcat(written, ",extended,0");
	strcat(written, address);
	strcat(written, "\n");
	strcpy(write_arr[write_pos], written);
	has_written = 1;
	write_pos++;
}

void change16bit(char *address, char *value) {
	checkvalsign(value);
	switch (sign) {
	case 0:
		strcat(written, "patch=1,EE,3020");
		break;
	case 1:
		strcat(written, "patch=1,EE,3030");
		break;
	}
	strcat(written, value);
	strcat(written, ",extended,0");
	strcat(written, address);
	strcat(written, "\n");
	strcpy(write_arr[write_pos], written);
	has_written = 1;
	write_pos++;
}

void change32bit(char *address, char *value) {
	checkvalsign(value);
	switch (sign) {
	case 0:
		strcat(written, "patch=1,EE,30400000");
		break;
	case 1:
		strcat(written, "patch=1,EE,30500000");
		break;
	}
	strcat(written, ",extended,0");
	strcat(written, address);
	strcat(written, "\n");
	strcpy(write_arr[write_pos], written);
	memcpy(written, &nullarr[0], 256);
	write_pos++;
	if (comment == 1) {
		strcat(written, "//");
	}
	else {
		incrementcounters();
	}
	strcat(written, "patch=1,EE,");
	strcat(written, value);
	strcat(written, ",extended,00000000\n");
	strcpy(write_arr[write_pos], written);
	has_written = 1;
	write_pos++;
}

void loopwrite(char *address, char *b, char *t, char *value) {
	strcat(written, "patch=1,EE,4");
	strcat(written, address);
	strcat(written, ",extended,");
	strcat(written, b);
	strcat(written, t);
	strcat(written, "\n");
	strcpy(write_arr[write_pos], written);
	memcpy(written, &nullarr[0], 256);
	write_pos++;
	if (comment == 1) {
		strcat(written, "//");
	}
	else {
		incrementcounters();
	}
	strcat(written, "patch=1,EE,");
	strcat(written, value);
	strcat(written, ",extended,00000000\n");
	strcpy(write_arr[write_pos], written);
	has_written = 1;
	write_pos++;
}

void writencopy(char *fromaddress, char *bytes, char *toaddress) {
	strcat(written, "patch=1,EE,5");
	strcat(written, fromaddress);
	strcat(written, ",extended,");
	addzero(bytes, 8);
	strcat(written, bytes);
	strcat(written, "\n");
	strcpy(write_arr[write_pos], written);
	memcpy(written, &nullarr[0], 256);
	write_pos++;
	if (comment == 1) {
		strcat(written, "//");
	}
	else {
		incrementcounters();
	}
	strcat(written, "patch=1,EE,0");
	strcat(written, toaddress);
	strcat(written, ",extended,00000000\n");
	strcpy(write_arr[write_pos], written);
	has_written = 1;
	write_pos++;
}

void writetopoint(char *A, char *O, char *bitdepth, char *value) {
	strcat(written, "patch=1,EE,6");
	strcat(written, A);
	strcat(written, ",extended,");
	addzero(value, 8);
	strcat(written, value);
	strcat(written, "\n");
	strcpy(write_arr[write_pos], written);
	memcpy(written, &nullarr[0], 256);
	write_pos++;
	if (comment == 1) {
		strcat(written, "//");
	}
	else {
		incrementcounters();
	}
	strcat(written, "patch=1,EE,000");
	if (strcmp(bitdepth, "32") == 0) {
		strcat(written, "2");
	}
	else if (strcmp(bitdepth, "16") == 0) {
		strcat(written, "1");
	}
	else if (strcmp(bitdepth, "8") == 0) {
		strcat(written, "0");
	}
	strcat(written, "0000,extended,");
	strcat(written, O);
	strcat(written, "\n");
	strcpy(write_arr[write_pos], written);
	has_written = 1;
	write_pos++;
}

void writelogic(char *address, char *logic, char *value) {
	strcat(written, "patch=1,EE,7");
	strcat(written, address);
	strcat(written, ",extended,000");
	if (strcmp(logic, "8OR") == 0) {
		strcat(written, "0");
	}
	else if (strcmp(logic, "16OR") == 0) {
		strcat(written, "1");
	}
	else if (strcmp(logic, "8AND") == 0) {
		strcat(written, "2");
	}
	else if (strcmp(logic, "16AND") == 0) {
		strcat(written, "3");
	}
	else if (strcmp(logic, "8XOR") == 0) {
		strcat(written, "4");
	}
	else if (strcmp(logic, "8XOR") == 0) {
		strcat(written, "5");
	}
	addzero(value, 4);
	strcat(written, value);
	strcat(written, "\n");
	strcpy(write_arr[write_pos], written);
	has_written = 1;
	write_pos++;
}

void writecopyfrompoint(char *A, char *O, char *toaddress) {
	strcat(written, "patch=1,EE,8");
	strcat(written, A);
	strcat(written, ",extended,");
	strcat(written, O);
	strcpy(write_arr[write_pos], written);
	strcat(written, "\n");
	memcpy(written, &nullarr[0], 256);
	write_pos++;
	if (comment == 1) {
		strcat(written, "//");
	}
	else {
		incrementcounters();
	}
	strcat(written, "patch=1,EE,0");
	strcat(written, toaddress);
	strcat(written, ",extended,00000000\n");
	strcpy(write_arr[write_pos], written);
	has_written = 1;
	write_pos++;
}

void writecopybetween(char *fromaddress,  char *toaddress) {
	strcat(written, "patch=1,EE,9");
	strcat(written, fromaddress);
	strcat(written, ",extended,0");
	strcat(written, toaddress);
	strcat(written, "\n");
	strcpy(write_arr[write_pos], written);
	has_written = 1;
	write_pos++;
}

void writecopytopoint(char *fromaddress, char *P, char *I) {
	strcat(written, "patch=1,EE,A");
	strcat(written, fromaddress);
	strcat(written, ",extended,00000000\n");
	strcpy(write_arr[write_pos], written);
	memcpy(written, &nullarr[0], 256);
	write_pos++;
	if (comment == 1) {
		strcat(written, "//");
	}
	else {
		incrementcounters();
	}
	strcat(written, "patch=1,EE,0");
	strcat(written, P);
	strcat(written, ",extended,0");
	strcat(written, I);
	strcpy(write_arr[write_pos], written);
	has_written = 1;
	write_pos++;
}

void writetimer(char *time) {
	memcpy(written, &nullarr[0], 256);
	memcpy(working_str, &nullarr[0], 256);
	strcat(written, "patch=1,EE,B");
	itoa(line_count1, working_str, 10);
	addzero(working_str, 2);
	strcat(written, working_str);
	addzero(time, 5);
	strcat(written, time);
	strcat(written, ",extended,00000000");
	strcat(written, "\n");
	strcpy(write_arr[timerpos], written);
	line_count1 = 9999;
	timercounting = 0;
	has_written = 1;
}

void writeif(char *address, char *operation, char *bitdepth, char *value, int position, int linecount) {
	memcpy(written, &nullarr[0], 256);
	memcpy(working_str, &nullarr[0], 256);
	strcat(written, "patch=1,EE,E");
	if (strcmp(bitdepth, "16") == 0) {
		strcat(written, "0");
	}
	else if (strcmp(bitdepth, "8") == 0) {
		strcat(written, "1");
	}
	itoa(linecount, working_str, 10);
	addzero(working_str, 2);
	strcat(written, working_str);
	addzero(value, 4);
	strcat(written, value);
	strcat(written, ",extended,");
	if (strcmp(operation, "==") == 0) {
		strcat(written, "0");
	}
	else if (strcmp(operation, "!=") == 0) {
		strcat(written, "1");
	}
	else if (strcmp(operation, "<") == 0) {
		strcat(written, "2");
	}
	else if (strcmp(operation, ">") == 0) {
		strcat(written, "3");
	}
	strcat(written, address);
	strcpy(write_arr[position], written);
	has_written = 1;
}

void writeifaddr(char *address, char *operation, char *bitdepth, char *address2, int position, int linecount) {
	memcpy(written, &nullarr[0], 256);
	memcpy(working_str, &nullarr[0], 256);
	strcat(written, "patch=1,EE,C");
	strcat(written, address);
	strcat(written, ",extended,00000000");
	strcat(written, "\n");
	strcpy(write_arr[position], written);
	position++;
	memcpy(written, &nullarr[0], 256);
	strcat(written, "patch=1,EE,0");
	strcat(written, address2);
	strcat(written, ",extended,");
	if (strcmp(bitdepth, "32") == 0) {
		strcat(written, "0");
	}
	else if (strcmp(bitdepth, "16") == 0) {
		strcat(written, "1");
	}
	else if (strcmp(bitdepth, "8") == 0) {
		strcat(written, "2");
	}
	if (strcmp(operation, "==") == 0) {
		strcat(written, "00");
	}
	else if (strcmp(operation, "!=") == 0) {
		strcat(written, "10");
	}
	else if (strcmp(operation, "<") == 0) {
		strcat(written, "20");
	}
	else if (strcmp(operation, ">") == 0) {
		strcat(written, "30");
	}
	itoa(linecount, working_str, 10);
	addzero(working_str, 5);
	strcat(written, working_str);
	strcat(written, "\n");
	strcpy(write_arr[position], written);
	has_written = 1;
}

void writeif1(char *address, char *operation, char *value) {
	memcpy(written, &nullarr[0], 256);
	strcat(written, "patch=1,EE,D");
	strcat(written, address);
	strcat(written, ",extended,00");
	if (strcmp(operation, "==")==0) {
		strcat(written, "00");
	}
	else if (strcmp(operation, "!=") == 0) {
		strcat(written, "10");
	}
	else if (strcmp(operation, "<") == 0) {
		strcat(written, "20");
	}
	else if (strcmp(operation, ">") == 0) {
		strcat(written, "30");
	}
	addzero(value, 4);
	has_written = 1;
	write_pos++;
}

void writebracket() {
	memcpy(linecopy, &nullarr[0], 256);
	int i;
	int countpos = 0;
	int countarray[] = { line_count1, line_count2, line_count3, line_count4, line_count5 };
	int smallest = countarray[0];
	for (i = 0; i < 5; ++i) {
		if (smallest > countarray[i]) {
			countpos = i;
			smallest = countarray[i];
		}
	}
	switch (countpos) {
	case 0:
		strcpy(linecopy, timerline);
		get1str();
		writetimer(value);
		break;
	case 1:
		strcpy(linecopy, ifline1);
		get4str();
		writeif(address, str1, str2, value, ifpos1, line_count2);
		line_count2 = 9999;
		ifcounting1 = 0;
		break;
	case 2:
		strcpy(linecopy, ifline2);
		get4str();
		writeif(address, str1, str2, value, ifpos2, line_count3);
		line_count3 = 9999;
		ifcounting2 = 0;
		break;
	case 3:
		strcpy(linecopy, ifaddrline1);
		get4str();
		writeifaddr(address, str1, str2, value, ifaddrpos1, line_count4);
		line_count4 = 9999;
		ifaddrcounting1 = 0;
		break;
	case 4:
		strcpy(linecopy, ifaddrline2);
		get4str();
		writeifaddr(address, str1, str2, value, ifaddrpos2, line_count5);
		line_count5 = 9999;
		ifaddrcounting2 = 0;
		break;
	}
}

void checkwrite() {
	if (strcmp(subbuff, "write8bit") == 0) {
		get2str();
		write8bit(address, value);
	}
	else if (strcmp(subbuff, "write16bit") == 0) {
		get2str();
		write16bit(address, value);
	}
	else if (strcmp(subbuff, "write32bit") == 0) {
		get2str();
		write32bit(address, value);
	}
	else if (strcmp(subbuff, "change8bit") == 0) {
		get2str();
		change8bit(address, value);
	}
	else if (strcmp(subbuff, "change16bit") == 0) {
		get2str();
		change16bit(address, value);
	}
	else if (strcmp(subbuff, "change32bit") == 0) {
		get2str();
		change32bit(address, value);
	}
	else if (strcmp(subbuff, "loopwrite") == 0) {
		get4str();
		loopwrite(address, str1, str2, value);
	}
	else if (strcmp(subbuff, "copynbyte") == 0) {
		get3str();
		writencopy(address, str1, value);
	}
	else if (strcmp(subbuff, "pointwrite") == 0) {
		get4str();
		writetopoint(address, str1, str2, value);
	}
	else if (strcmp(subbuff, "logicwrite") == 0) {
		get3str();
		writelogic(address, str1, value);
	}
	else if (strcmp(subbuff, "copyfrompoint") == 0) {
		get3str();
		writecopyfrompoint(address, str1, value);
	}
	else if (strcmp(subbuff, "copyaddr2addr") == 0) {
		get2str();
		writecopybetween(address, value);
	}
	else if (strcmp(subbuff, "copytopoint") == 0) {
		get3str();
		writecopytopoint(address, str1, value);
	}
	else if (strcmp(subbuff, "timeractivate") == 0) {
		memcpy(timerline, &nullarr[0], 256);
		strcpy(timerline, linecopy);
		line_count1 = 0;
		timerpos = write_pos;
		timercounting = 1;
		write_pos++;
	}
	else if (strcmp(subbuff, "ifaddr") == 0) {
		if (ifaddrcounting1 == 0) {
			memcpy(ifaddrline1, &nullarr[0], 256);
			strcpy(ifaddrline1, linecopy);
			line_count4 = 0;
			ifaddrpos1 = write_pos;
			ifaddrcounting1 = 1;
			write_pos+=2;
		}
		else {
			memcpy(ifaddrline2, &nullarr[0], 256);
			strcpy(ifaddrline2, linecopy);
			line_count5 = 0;
			ifaddrpos2 = write_pos;
			ifaddrcounting2 = 1;
			write_pos+=2;
		}
	}
	else if (strcmp(subbuff, "if1") == 0) {
		get3str();
		writeif1(address, str1, value);
	}
	else if (strcmp(subbuff, "if") == 0) {
		if (ifcounting1 == 0) {
			memcpy(ifline1, &nullarr[0], 256);
			strcpy(ifline1, linecopy);
			line_count2 = 0;
			ifpos1 = write_pos;
			ifcounting1 = 1;
			write_pos++;
		}
		else {
			memcpy(ifline2, &nullarr[0], 256);
			strcpy(ifline2, linecopy);
			line_count3 = 0;
			ifpos2 = write_pos;
			ifcounting2 = 1;
			write_pos++;
		}
	}
}

int main(int argc, char* argv[])
{
	char const* const fileName = argv[1];
	FILE* file = fopen(fileName, "r");


	while (fgets(line, sizeof(line), file)) {
		has_written = 0;
		comment = 0;
		memcpy(written, &nullarr[0], 256);
		memcpy(subbuff, &nullarr[0], 256);
		memcpy(working_str, &nullarr[0], 256);
		memcpy(str1, &nullarr[0], 256);
		memcpy(str2, &nullarr[0], 256);
		memcpy(linecopy, &line[0], sizeof(line));
		remove_spaces(linecopy);
		if (strcmp(&linecopy[0], "}") == 0) {
			writebracket();
		}
		operand_pos = strcspn(linecopy, keys);
		memcpy(subbuff, &linecopy[0], operand_pos);
		memcpy(operand, &linecopy[operand_pos], 1);
		subbuff[operand_pos] = '\0';
		operand[1] = '\0';
		memcpy(working_str, &linecopy[0], 2);
		if (strcmp(working_str, "//") != 0) {
			incrementcounters();
		}
		if (strcmp(working_str, "//") == 0) {
			memcpy(subbuff, &nullarr[0], 256);
			memcpy(subbuff, &linecopy[2], operand_pos-2);
			strcat(written, "//");
			comment = 1;
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
				addvar(subbuff);
				strcpy(var_arr[var_store_len], value);
				var_store_len++;
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

