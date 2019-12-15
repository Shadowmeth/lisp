#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <editline/readline.h>

typedef struct Token {

	enum Token_Type {
		PLUS, MINUS, MULTIPLY, DIVIDE, NUMBER, L_PAREN, R_PAREN, INVALID, _EOF

	}T_Type;

	union {
		long long i;
		int sym; /* +, -, *, /, (, ), and -1 for EOF */
	}T_Val;

}Token;

bool match(int ch, char *src) 
{
	if (*src == ch) {
		src++;
		return true;
	}

	return false;
}

Token lex(char *src)
{

	Token ret_tok;

	while (*src == ' ' || *src == '\t') {	/* skip whitespace */
		src++;
	}

	if (!*src) {
		ret_tok.T_Type = _EOF;
		ret_tok.T_Val.sym = -1;
		src++;
		return ret_tok;

	} else if (*src == '+') {
		ret_tok.T_Type = PLUS;
		ret_tok.T_Val.sym = '+';
		src++;
		return ret_tok;
	
	} else if (*src == '-') {
		ret_tok.T_Type = MINUS;
		ret_tok.T_Val.sym = '*';
		src++;
		return ret_tok;
	
	} else if (*src == '/') {
		ret_tok.T_Type = DIVIDE;
		ret_tok.T_Val.sym = '/';
		src++;
		return ret_tok;

	} else if (*src == '*') {
		ret_tok.T_Type = MULTIPLY;
		ret_tok.T_Val.sym = '*';
		src++;
		return ret_tok;

	} else if (*src == '(') {
		ret_tok.T_Type = L_PAREN;
		ret_tok.T_Val.sym = '(';
		src++;
		return ret_tok;

	} else if (*src == ')') {
		ret_tok.T_Type = R_PAREN;
		ret_tok.T_Val.sym = ')';
		src++;
		return ret_tok;

	} else if (isdigit(*src)) { /* we are not considering overflow yet */
		/* found a number, collect it */
		int num = 0;
		while (isdigit(*src)) {
			num = num * 10 + (*src - '0');
			src++;
		}

		ret_tok.T_Type = NUMBER;
		ret_tok.T_Val.i = num; 

		return ret_tok;
	}

	/* If we reached here, we found some invalid characters */
	/* No need to consider the value of the token */
	ret_tok.T_Type = INVALID;
	return ret_tok;
}

bool number(char *src)
{
	Token t = lex(src);
	if (t.T_Type == NUMBER) {
		return true;
	} else if (t.T_Type != NUMBER && t.T_Type != INVALID) {
		fprintf(stderr, "Error: expected number, found %c\n", t.T_Val.sym);
		return true;
	} else if (t.T_Type == INVALID) {
		fprintf(stderr, "Invalid characters found!\n");
		return false;

	}

	return true;
}

bool operator(char *src)
{
	Token t = lex(src);

	if (t.T_Type == _EOF) {
		fprintf(stderr, "End of line found!\n");
		return false;
	}

	if (t.T_Type == INVALID) {
		fprintf(stderr, "Invalid characters found!\n");
		return false;
	}

	if (t.T_Type != PLUS && t.T_Type != MINUS && t.T_Type != MULTIPLY && t.T_Type != DIVIDE) {
		fprintf(stderr, "Error: Expected an operator (+, -, *, /) - found ");
		if (t.T_Type == _EOF) {
			fprintf(stderr, "%d\n", -1); 
		} else if (t.T_Type == NUMBER) {
			fprintf(stderr, "%d\n", t.T_Val.i);
		} else {
			fprintf(stderr, "%c\n", t.T_Val.sym);
		}
		
		return false;
	}

	return true;

}

bool expr(char *src)
{
	Token t = lex(src);

	if (t.T_Type == _EOF)  {
		fprintf(stderr, "End of line found!\n");
		return false;
	}

	if (t.T_Type == INVALID) {
		fprintf(stderr, "Invalid characters found!\n");
		return false;	

	}
	
	if (t.T_Type == NUMBER) {
		return true;
	}


	if (!match('(', src)) {
		fprintf(stderr, "Error: Expected '(', found %c\n", t.T_Val.sym);
		return false;
	}


	if (!operator(src)) {
		return false;
	}

	do {
		if (!operator(src)) {
			return false;
		}

	} while (expr(src));

	if (!match(')', src)) {
		fprintf(stderr, "Error: expected ')', found %c\n", t.T_Val.sym);
		return false;
	}

	return true;
}

bool lispy(char *src)
{
	if (!operator(src)) {
		return false;
	}

	do {
		if (!expr(src)) {
			return false;
		}

	} while(expr(src));

	return true;
}

int main(int argc, char *argv[])
{
	puts("Lispy version 0.0.1");
	puts("Press Ctrl+c to exit\n");


	while (1) {
		char *input = readline("lispy> ");

		add_history(input);
		char *free_input = input; /* save address to free the memory, because input is changed */
		
		lispy(input);
		
		free(free_input);
	}

	return 0;
}

