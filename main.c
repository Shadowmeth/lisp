/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description: Lisp compiler 
 *
 *        Version:  1.0
 *        Created:  12/09/2019 09:21:03 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (Syed Kumail Hussain Sheraz), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <editline/readline.h>

int lookahead = 0; /* Start at the first char of input */
char *input;

struct ast_node {
	enum Node_Type {
		NUM,
		OP,
	}type;

	union {
		int val;
		int op;
	}v;
	
	struct ast_node *left;
	struct ast_node *right;

};

struct ast_node *new_op_node(int op)
{
	struct ast_node *new = malloc(sizeof(struct ast_node));
	new -> type = OP;
	new -> v.op = op;

	new -> left = NULL;
	new -> right = NULL;

	return new;
}

bool number(struct ast_node *n)
{
	bool num_found = false;
	int v = 0;

	if (match('-'))  {	/* negative number, we assume that a number is found after -, if there isn't any, assume 0 */ 

		while (isdigit(input[lookahead])) {
			v = v * 10 + input[lookahead] - '0';
			lookahead++;
		}
		num_found = true;
	
	} else if (isdigit(input[lookahead])) {	/* positive number */

		while (isdigit(input[lookahead])) {
			v = v * 10 + input[lookahead] - '0';
			lookahead++;
		}

		num_found = true;
	}

	if (num_found) {
		/* go to the left most leaf of n and add a number */
			

	}
}

bool match(int ch)
{
	if (input[lookahead] == ch) {
		/* advance lookahead and return true */
		lookahead++;
		return true;
	}

	return false;
}

void operator(struct ast_node  *n)
{
	/* skip extra space */
	while (isspace(input[lookahead]))
		lookahead++;

	if (match('+')) {
		n -> left = NULL;
		n -> right = new_op_node('+');
	} else if (match('-')) {
		n -> left = NULL;
		n -> right = new_op_node('-');
	} else if (match('/')) {
		n -> left = NULL;
		n -> right = new_op_node('/');
	} else if (match('*')) {
		n -> left = NULL;
		n -> right = new_op_node('*');
	} else {	/* error */
		fprintf(stderr, "Error: expected (+, -, /, *) - got %c\n", input[lookahead]);
		exit(EXIT_FAILURE);
	}
}

bool expr(struct ast_node *n)
{

	if (number(n)) { /* if a number is found add to the left of n as a new ast_node */
		return true;
	} else {
		if (!match('(')); {
			fprintf(stderr, "Error: expected '(', got %c\n", input[lookahead]);
			exit(EXIT_FAILURE);
		}
		operator(n);

		while(expr(n))
			expr(n);
	
		if (!match(')')) {
			fprintf(stderr, "Error: expected ')', got %c\n", input[lookahead]);
			exit(EXIT_FAILURE);
		}
	}
}

int main(int argc, char *argv[])
{
	/* Print version and exit info */
	puts("Lispy version 0.0.1");
	puts("Press Ctrl+c to exit\n");

	struct ast_node *prog = malloc(sizeof(struct ast_node));

	while (1) {

		input = readline("lispy> ");

		/* Add input to history */
		add_history(input);
	
		operator(prog);

		while(expr(prog))
			expr(prog);

		free(input);

	}

	return 0;
}

